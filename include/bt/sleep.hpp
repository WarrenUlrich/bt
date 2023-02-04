#pragma once

#include <chrono>

#include <bt/node_task.hpp>
#include <bt/yield.hpp>

namespace bt {
  template <typename Rep, typename Period>
  node_task sleep_for(std::chrono::duration<Rep, Period> duration) {
    const auto now = std::chrono::steady_clock::now();
    const auto end = now + duration;

    while (std::chrono::steady_clock::now() < end)
      co_await yield();
    
    co_return node_status::success;
  }

  template <typename Clock, typename Duration>
  node_task sleep_until(std::chrono::time_point<Clock, Duration> time_point) {
    while (std::chrono::steady_clock::now() < time_point)
      co_await yield();
    
    co_return node_status::success;
  }

  template <typename Predicate>
  node_task sleep_until(Predicate&& predicate) {
    while (!predicate())
      co_await yield();
    
    co_return node_status::success;
  }

  template <typename Rep, typename Period, typename Predicate>
  node_task sleep_until(std::chrono::duration<Rep, Period> duration, Predicate&& predicate) {
    const auto now = std::chrono::steady_clock::now();
    const auto end = now + duration;

    while (std::chrono::steady_clock::now() < end) {
      if (predicate())
        co_return node_status::success;
      
      co_await yield();
    }
    
    co_return node_status::failure;
  }
}