#include <iostream>
#include "solution.h"

// Select items which have S.first in range [lower..upper]
std::size_t select(std::array<S, N> &output, const std::array<S, N> &input,
                   const std::uint32_t lower, const std::uint32_t upper) {
  std::size_t count = 0;
  for (const auto item : input) {
    bool cond = (lower <= item.first) && (item.first <= upper);
    S temp[2] = {{}, item};
    output[count] = std::move(temp[cond]);
    count = count + cond;
  }
  return count;
}
