#include "hyundeok/utils/math.h"
#include "hyundeok/types.h"

#include <assert.h>  // assert
#include <stdbool.h> // bool, true, false

static bool IsPrime(UllT n) {
  if (n < 2)
    return false;

  if (n < 4)
    return true;

  if (n % 2 == 0)
    return false;

  // Square is usually cheaper than square root
  for (UllT i = 3; i * i <= n; i += 2) {
    if (n % i == 0)
      return false;
  }

  return true;
}

UllT GetPrimeNumber(UllT lower_bound) {
  assert(lower_bound > 1);

  const UllT upper_bound = -1;

  for (UllT i = lower_bound; i < upper_bound; ++i) {
    if (IsPrime(i))
      return i;
  }

  return 0;
}
