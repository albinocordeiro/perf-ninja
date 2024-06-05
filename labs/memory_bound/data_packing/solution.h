
#include <array>

// Assume those constants never change
constexpr int N = 10000;
constexpr int minRandom = 0;
constexpr int maxRandom = 100;

// FIXME: this data structure can be reduced in size
struct S {
  bool b;
  bool operator<(const S &s) const { return this->i < s.i; }
  unsigned s : 8;
  unsigned i : 8;
  short l;
  float d;
};

void init(std::array<S, N> &arr);
S create_entry(int first_value, int second_value);
void solution(std::array<S, N> &arr);
