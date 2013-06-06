#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <cmath>

uint64_t next_pow2(uint64_t n) {
  if (n == 0)
    return 1;

  n--;
  uint64_t np2 = 1;
  while (n > 0) {
    np2 <<= 1;
    n >>= 1;
  }
  return np2;
}

uint64_t calc_key(uint64_t index, uint64_t maximum) {
  uint64_t np2 = next_pow2(maximum+1);
  uint64_t level = floor(log2(index));
  uint64_t level_start = ((uint64_t)1) << level;
  uint64_t slot = index - level_start;
  uint64_t step_size = np2 >> level;
  uint64_t offset = step_size >> 1;

  return step_size * slot + offset;
}

int main(int argc, char * argv[]) {
  uint64_t n = atol(argv[1]);
  uint64_t index;
  uint64_t key = 1;
  uint64_t height = 16;


  // Create an AVL tree with a bunch of elements initially.
  for (index = 1; index <= ((1 << height) - 1) && index <= n; index++) {
    printf("I %lu\n", calc_key(key++, (1 << height) - 1));
  }

  // Now generate a sequence of increasing-cost blocks, where each
  // block "counts up" how expensive the operations contained are.
  uint64_t blockSize = ceil(sqrt(n / height));
  uint64_t inBlockIndex = 1;
  uint64_t blockIndex = 1;
  for ( ; index <= n; index++) {
    uint64_t slot = (blockIndex-1) / height + 1;
    if (inBlockIndex > slot) {
      // Cheap key.
      key = calc_key(1, (1 << height) - 1);
    }
    else if (inBlockIndex == slot) {
      // Varying cost key.
      uint64_t slotOffset = blockIndex - ((slot - 1) * height) - 1;
      key = calc_key(1 << slotOffset, (1 << height) - 1);
    }
    else {
      // Expensive key.
      key = calc_key(1 << (height - 1), (1 << height) - 1);
    }
    inBlockIndex++;
    if (inBlockIndex > blockSize) {
      inBlockIndex = 1;
      blockIndex++;
    }

    printf("F %lu\n", key);
  }
}
