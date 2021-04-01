#include "hyundeok/cache/timed_lru.h"
#include "hyundeok/macro.h"

typedef struct LRUData {
  void* key_;
  void* value_;
  struct timeval timestamp_;
} LRUData;

static LRUData*
LRUDataCreate(void* key, void* value) {
  LRUData* data = malloc(sizeof(LRUData));

  data->key_ = key;
  data->value_ = value;
  gettimeofday(&data->timestamp_, NULL);

  return data;
}

static DllnT*
LRUDataInsert(DoublyLinkedList* lru, void* key, void* value) {
  LRUData* data = LRUDataCreate(key, value);

  // the most recently used node is placed at the back
  return DoublyLinkedListAppend(lru, data);
}

static void
LRUDataDestroyValue(LRUData* data, GDestroyNotify value_destructor) {
  if (value_destructor != NULL)
    value_destructor(data->value_);
}

static long
LRUDataGetTimestamp(LRUData* data) {
  return data->timestamp_.tv_sec;
}

static void
LRUDataUpdateTimestamp(LRUData* data) {
  gettimeofday(&data->timestamp_, NULL);
}

static void
LRUDataUpdateNode(DoublyLinkedList* lru, DllnT* node) {
  DoublyLinkedListRemove(lru, node, 0);
  node->prev_ = NULL;
  node->next_ = NULL;
  LRUDataUpdateTimestamp(node->data_);
  DoublyLinkedListAppendNode(lru, node);
}

LRUCache*
LRUCacheNew(size_t max_size, long duration, GDestroyNotify key_destructor,
            GDestroyNotify value_destructor) {
  LRUCache* cache = malloc(sizeof(LRUCache));
  HYUNDEOK_MEMORY_ASSERT(cache, NULL);

  cache->table_ = g_hash_table_new_full(NULL, NULL, key_destructor, NULL);

  // if allocation of hash table fails, deallocate previous heap
  HYUNDEOK_MEMORY_ASSERT(cache, free(cache));

  cache->lru_ = DoublyLinkedListNew();

  // if allocation of LRU list fails, deallocate previous heap
  HYUNDEOK_MEMORY_ASSERT(
      cache, do {
        free(cache->table_);
        free(cache);
      } while (0));

  int result = pthread_mutex_init(&cache->lock_, NULL);

  // if pthread_mutex_init fails, deallocate previous heap
  HYUNDEOK_ASSERT(
      result, 0, NULL, do {
        free(cache->lru_);
        free(cache->table_);
        free(cache);
      } while (0));

  cache->max_size_ = max_size;
  cache->duration_ = duration;
  cache->value_destructor_ = value_destructor;

  return cache;
}

void
LRUCacheDestroy(LRUCache* cache) {
  pthread_mutex_lock(&cache->lock_);

  g_hash_table_destroy(cache->table_);

  // Why not DoublyLinkedListDestroy?
  //
  // That method deallocates the node and data, which is LRUData in this case.
  // But the real data lies in LRUData that must be freed. This real data is
  // the actual value of a cache entry.
  for (DllnT* head = cache->lru_->head_; head != NULL;) {
    DllnT* next = head->next_;
    LRUData* data = next->data_;
    LRUDataDestroyValue(data, cache->value_destructor_);
    free(data);
    free(head);
    head = next;
  }

  free(cache->lru_);

  pthread_mutex_unlock(&cache->lock_);
  pthread_mutex_destroy(&cache->lock_);
  free(cache);
}

int
LRUCacheIsFull(LRUCache* cache) {
  return cache->max_size_ == g_hash_table_size(cache->table_);
}

int
LRUCacheInsert(LRUCache* cache, void* key, void* value) {
  pthread_mutex_lock(&cache->lock_);

  if (LRUCacheIsFull(cache)) {
    // If cache is full, remove the least recently used entry.
    DoublyLinkedList* list = cache->lru_;
    DllnT* lru = list->head_;
    LRUData* data = lru->data_;

    g_hash_table_remove(cache->table_, data->key_);
    LRUDataDestroyValue(data, cache->value_destructor_);
    DoublyLinkedListRemove(list, lru, 1);
  }

  DllnT* lookup = g_hash_table_lookup(cache->table_, key);

  // if key already exists, append it to the list, making it most recently used
  // entry.
  if (value) {
    LRUDataUpdateNode(cache->lru_, lookup);
    pthread_mutex_unlock(&cache->lock_);
    return 0;
  }

  LRUDataInsert(cache->lru_, key, value);
  g_hash_table_insert(cache->table_, key, cache->lru_->tail_);

  pthread_mutex_unlock(&cache->lock_);

  return 0;
}

void
LRUCacheRemove(LRUCache* cache, void* key) {
  pthread_mutex_lock(&cache->lock_);

  DllnT* value = g_hash_table_lookup(cache->table_, key);

  if (value == NULL) {
    pthread_mutex_unlock(&cache->lock_);
    return;
  }

  // This process is required as the actual data end users passed to
  // LRUCacheInsert lies in LRUData*. To allow end users to deallocate the
  // values of the cache entries, those values should be extracted from the list
  // node.
  LRUData* data = value->data_;
  LRUDataDestroyValue(data, cache->value_destructor_);
  DoublyLinkedListRemove(cache->lru_, value, 1);

  pthread_mutex_unlock(&cache->lock_);
}

void*
LRUCacheFind(LRUCache* cache, void* key) {
  pthread_mutex_lock(&cache->lock_);

  // Same as the explanation in LRUCacheRemove
  DllnT* value = g_hash_table_lookup(cache->table_, key);

  if (value == NULL) {
    pthread_mutex_unlock(&cache->lock_);
    return NULL;
  }

  LRUData* data = value->data_;
  void* real_value = data->value_;

  LRUDataUpdateNode(cache->lru_, value);

  pthread_mutex_unlock(&cache->lock_);

  return real_value;
}
