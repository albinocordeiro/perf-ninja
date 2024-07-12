
#include "solution.h"
#include <memory>
#include <iostream>
#include <string_view>

// Make identity matrix
void identity(Matrix &result) {
  memset(&result, 0, sizeof(result));

  for (int i = 0; i < N; i++) {
    result[i][i] = 1;
  }
}

// Multiply two square matrices
void multiply(Matrix &result, const Matrix &a, const Matrix &b) {
  zero(result);

  int i{}, k{}, j{};

  //TODO: Probably SIMD is worth, but I'm not familiar with it yet. Come back here

  for (; i < N; i+=2) {
    for (k = 0; k < N; k++) {
      for (j = 0; j < N; j++) {
        result[i][j] += a[i][k] * b[k][j];
        result[i+1][j] += a[i+1][k] * b[k][j];
      }
    }
  }
  
}

// Compute integer power of a given square matrix
Matrix power(const Matrix &input, const uint32_t k) {
  // Temporary products
  std::unique_ptr<Matrix> productCurrent(new Matrix());
  std::unique_ptr<Matrix> productNext(new Matrix());

  // Temporary elements = a^(2^integer)
  std::unique_ptr<Matrix> elementCurrent(new Matrix());
  std::unique_ptr<Matrix> elementNext(new Matrix());

  // Initial values
  identity(*productCurrent);
  *elementCurrent = input;

  // Use binary representation of k to be O(log(k))
  for (auto i = k; i > 0; i >>= 1) {
    if (i & 1) {
      // Multiply the product by element
      multiply(*productNext, *productCurrent, *elementCurrent);
      std::swap(productNext, productCurrent);

      // Exit early to skip next squaring
      if (!(i ^ 1))
        break;
    }

    // Square an element
    multiply(*elementNext, *elementCurrent, *elementCurrent);
    std::swap(elementNext, elementCurrent);
  }

  return std::move(*productCurrent);
}
