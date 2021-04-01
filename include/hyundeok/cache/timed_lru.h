#ifndef HYUNDEOK_CACHE_TIMED_LRU_H
#define HYUNDEOK_CACHE_TIMED_LRU_H

#include "hyundeok/structures/doubly_linked_list.h"

#include <glib.h>     // GHashTable
#include <pthread.h>  // pthread_mutex*
#include <sys/time.h> // gettimeofday
#include <stddef.h>

/**
 * @struct LRUCache
 * @brief Time-based least recently used (LRU) cache with thread-safety
 * @var LRUCache::lock_
 * @var LRUCache::max_size_
 * @var LRUCache::duration_
 * @var LRUCache::table_
 * @var LRUCache::value_destructor_
 * @var LRUCache::lru_
 *
 * The hash table stores entries whose key and value are void* and DllnT*
 * respectively. DllnT is the type of node for a doubly linked list that
 * provides a LRU replacement policy.
 *
 * The doubly linked list stores key and value of the hash table and the
 * timestamp when its entry is created or accessed.
 *
 * If this structure is expressed in C++, it would resemble as below:
 *    GHashTable<KeyType, DllnT<KeyType, ValueType, long>> table_;
 *    DoublyLinkedList<DllnT<KeyType, ValueType, long>> lru_;
 *
 * However, end users will use it as if they are using the hash table of the
 * they provided.
 */
typedef struct LRUCache {
  pthread_mutex_t lock_;
  size_t max_size_;
  long duration_;
  GHashTable* table_;
  GDestroyNotify value_destructor_;
  DoublyLinkedList* lru_;
} LRUCache;

/**
 * Creates a cache.
 *
 * @param max_size Maximum size of cache
 * @param duration Number of seconds for which each cache entry will remain in
 *                 cache without access
 * @param key_destructor Destructor called on the key of cache entry upon its
 *                       removal
 * @param value_destructor Destructor called on the value of cache entry upon
 *                         its removal
 * @return Cache
 */
LRUCache*
LRUCacheNew(size_t max_size, long duration, GDestroyNotify key_destructor,
            GDestroyNotify value_destructor);

/**
 * Destroys a cache.
 *
 * @param cache Cache
 */
void
LRUCacheDestroy(LRUCache* cache);

/**
 * Checks whether a cache is full.
 *
 * @param cache Cache
 * @return 1 if full; otherwise, 0
 */
int
LRUCacheIsFull(LRUCache* cache);

/**
 * Inserts a cache entry to a cache.
 *
 * @param cache Cache
 * @param key Key of cache entry
 * @param value value of cache entry
 * @return 1 if success; otherwise, 0
 */
int
LRUCacheInsert(LRUCache* cache, void* key, void* value);

/**
 * Removes a cache entry from a cache.
 *
 * @param cache Cache
 * @param key Key of cache entry
 */
void
LRUCacheRemove(LRUCache* cache, void* key);

/**
 * Find a cache entry given a key.
 *
 * @param cache Cache
 * @param key Key of cache entry
 * @return Value of cache entry
 */
void*
LRUCacheFind(LRUCache* cache, void* key);

#endif
