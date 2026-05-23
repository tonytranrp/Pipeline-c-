#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

namespace pb::runtime {

class thread_pool {
public:
  explicit thread_pool(std::size_t num_threads = std::thread::hardware_concurrency()) {
    if (num_threads == 0) {
      num_threads = 1;
    }
    workers_.reserve(num_threads);
    for (std::size_t i = 0; i < num_threads; ++i) {
      workers_.emplace_back([this] {
        while (true) {
          std::function<void()> task;
          {
            std::unique_lock lock(mutex_);
            cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) return;
            task = std::move(tasks_.front());
            tasks_.pop();
          }
          task();
        }
      });
    }
  }

  ~thread_pool() {
    {
      std::unique_lock lock(mutex_);
      stop_ = true;
    }
    cv_.notify_all();
    for (auto& w : workers_) {
      if (w.joinable()) w.join();
    }
  }

  thread_pool(const thread_pool&) = delete;
  thread_pool& operator=(const thread_pool&) = delete;
  thread_pool(thread_pool&&) = delete;
  thread_pool& operator=(thread_pool&&) = delete;

  template <class F>
  auto enqueue(F&& f) -> std::future<decltype(f())> {
    using R = decltype(f());
    auto task = std::make_shared<std::packaged_task<R()>>(std::forward<F>(f));
    auto future = task->get_future();
    {
      std::unique_lock lock(mutex_);
      if (stop_) {
        throw std::runtime_error("enqueue on stopped thread_pool");
      }
      tasks_.emplace([task] { (*task)(); });
    }
    cv_.notify_one();
    return future;
  }


  template <class Iterator>
  auto enqueue_range(Iterator begin, Iterator end)
      -> std::vector<std::future<std::invoke_result_t<typename std::iterator_traits<Iterator>::value_type>>> {
    using R = std::invoke_result_t<typename std::iterator_traits<Iterator>::value_type>;
    std::vector<std::future<R>> futures;
    futures.reserve(std::distance(begin, end));

    {
      std::unique_lock lock(mutex_);
      if (stop_) {
        throw std::runtime_error("enqueue_range on stopped thread_pool");
      }

      for (auto it = begin; it != end; ++it) {
        auto task = std::make_shared<std::packaged_task<R()>>(*it);
        futures.push_back(task->get_future());
        tasks_.emplace([task] { (*task)(); });
      }
    }
    cv_.notify_all();
    return futures;
  }

  [[nodiscard]] std::size_t pending_tasks() const {
    std::unique_lock lock(mutex_);
    return tasks_.size();
  }

  [[nodiscard]] std::size_t worker_count() const noexcept { return workers_.size(); }

private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  mutable std::mutex mutex_;
  std::condition_variable cv_;
  bool stop_{false};
};

} // namespace pb::runtime

