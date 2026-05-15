#include <array>

int main() {
  std::array<int, 5> stages{1, 2, 3, 4, 5};
  int sum = 0;
  for (int value : stages) {
    sum += value;
  }
  return sum == 15 ? 0 : 1;
}
