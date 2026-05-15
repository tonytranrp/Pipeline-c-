#include <vector>
#include <string>

int main() {
  std::vector<std::string> pipeline = {"include", "compile", "time"};
  return static_cast<int>(pipeline.size());
}
