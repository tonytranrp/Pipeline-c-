#include <pb/pipeline.hpp>

struct read_bytes {
  using input_type = const char*;
  using output_type = int;
};

struct expects_text {
  using input_type = const char*;
  using output_type = void;
};

using invalid_pipeline = pb::from<const char*>::then<read_bytes>::to<expects_text>;

int main() {
  return sizeof(invalid_pipeline);
}
