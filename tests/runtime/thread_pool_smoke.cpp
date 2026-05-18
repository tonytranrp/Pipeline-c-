#include <pb/runtime/thread_pool.hpp>
#include <pb/pipeline.hpp>

#include <cassert>
#include <future>
#include <memory>
#include <vector>

int main() {
  // Test 1: Construct and destruct
  {
    pb::runtime::thread_pool pool{4};
    assert(pool.worker_count() == 4);
  }

  // Test 2: Enqueue a simple task
  {
    pb::runtime::thread_pool pool{2};
    auto future = pool.enqueue([] { return 42; });
    assert(future.get() == 42);
  }

  // Test 3: Run multiple tasks in parallel
  {
    pb::runtime::thread_pool pool{4};
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 8; ++i) {
      futures.push_back(pool.enqueue([i] { return i * i; }));
    }
    for (int i = 0; i < 8; ++i) {
      assert(futures[static_cast<std::size_t>(i)].get() == i * i);
    }
  }

  // Test 4: Move-only return types
  {
    pb::runtime::thread_pool pool{1};
    auto future = pool.enqueue([] { return std::make_unique<int>(7); });
    auto result = future.get();
    assert(*result == 7);
  }

  // Test 5: Void return type
  {
    pb::runtime::thread_pool pool{1};
    int counter = 0;
    auto future = pool.enqueue([&counter] { ++counter; });
    future.get();
    assert(counter == 1);
  }

  // Test 6: Default thread count (hardware_concurrency)
  {
    pb::runtime::thread_pool pool;
    assert(pool.worker_count() > 0);
  }

  return 0;
}
