#include "solution.hpp"
#include <array>
#include <iostream>

unsigned getSumOfDigits(unsigned n) {
  unsigned sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n * 0.1f;
  }
  return sum;
}

// Task: lookup all the values from l2 in l1.
// For every found value, find the sum of its digits.
// Return the sum of all digits in every found number.
// Both lists have no duplicates and elements placed in *random* order.
// Do NOT sort any of the lists. Do NOT store elements in a hash_map/sets.

// Hint: Traversing a linked list is a long data dependency chain:
//       to get the node N+1 you need to retrieve the node N first.
//       Think how you can execute multiple dependency chains in parallel.
unsigned solution(List *l1, List *l2) {
  unsigned retVal = 0;

  List *head2 = l2;

  // O(N^2) algorithm:
  while (l1) 
  {
    bool vFound{};
    bool v2Found{};
    
    unsigned v = l1->value;
    unsigned v2 = l1->next ? l1->next->value : 0;

    l2 = head2;
    while (l2) 
    {
      if (!vFound && l2->value == v) 
      {
        retVal += getSumOfDigits(v);
        vFound = true;
      }
      if(!v2Found && l2->value == v2)
      {
        retVal += getSumOfDigits(v2);
        v2Found = true;
      }

      if(vFound & v2Found) break;

      l2 = l2->next;
    }

    if(!l1->next) return retVal;
    l1 = l1->next->next;
  }

  return retVal;
}
