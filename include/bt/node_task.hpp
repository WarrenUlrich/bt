#pragma once

#include <bt/node_status.hpp>

namespace bt {
class node_task {
public:
  class promise_type {
  public:
    node_task get_return_object() noexcept {
      return node_task(handle_type::from_promise(*this));
    }

    std::suspend_always initial_suspend() const noexcept { return {}; }

    std::suspend_always final_suspend() const noexcept { return {}; }

    void unhandled_exception() const noexcept { std::terminate(); }

    std::suspend_always yield_value(node_status status) noexcept {
      _status = status;
      return {};
    }

    void return_value(node_status status) { _status = status; }

    node_status return_value() const { return _status; }

  private:
    node_status _status = node_status::success;
    std::coroutine_handle<> _continuation;

    friend node_task;
  };

  using handle_type = std::coroutine_handle<promise_type>;

  node_task() = default;

  node_task(handle_type handle) : _handle(handle) {}

  node_task(const node_task &) = delete;

  node_task(node_task &&other) : _handle(other._handle) {
    other._handle = nullptr;
  }

  node_task &operator=(const node_task &) = delete;

  node_task &operator=(node_task &&other) {
    if (_handle) {
      _handle.destroy();
    }

    _handle = std::exchange(other._handle, nullptr);
    return *this;
  }

  ~node_task() {
    if (_handle) {
      _handle.destroy();
    }
  }

  node_status tick() {
    _handle.resume();
    return _handle.promise().return_value();
  }

  bool done() const { return !_handle || _handle.done(); }

private:
  handle_type _handle;
};
} // namespace bt