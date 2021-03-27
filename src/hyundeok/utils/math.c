#include "hyundeok/utils/math.h"
#include "hyundeok/types.h"

#include <assert.h>  // assert
#include <stdbool.h> // bool, true, false
#include <math.h>    // sqrt

static bool IsPrime(UllT n) {
  if (n < 2)
    return false;

  if (n < 4)
    return true;

  if (n % 2 == 0)
    return false;

  const UllT upper_bound = sqrt(n) + 1;

  for (UllT i = 3; i <= upper_bound; i += 2) {
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
