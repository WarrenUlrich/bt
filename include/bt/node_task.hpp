#pragma once

#include <bt/node_status.hpp>
#include <coroutine>
#include <algorithm>
#include <utility>

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

    void return_value(node_status status) { _status = status; }

    node_status return_value() const {
      return _status;
    }

  private:
    node_status _status = node_status::running;
    std::coroutine_handle<> _awaiter;
    friend node_task;
  };

  class awaitable {
  public:
    constexpr awaitable() noexcept = default;
    
    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<promise_type> handle) noexcept {
      handle.promise()._awaiter = _awaiter;
    }

    node_status await_resume() const noexcept {
      return _awaiter.promise().return_value();
    }

  private:
    node_status _status = node_status::running;
    std::coroutine_handle<promise_type> _awaiter;

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
    if (!_handle) 
      return node_status::success;

    if (_handle.promise()._awaiter) {
      _handle.promise()._awaiter.resume();
      if (_handle.promise()._awaiter.done())
        _handle.promise()._awaiter = nullptr;
      
      return _handle.promise().return_value();
    } else {
      _handle.resume();
      return _handle.promise().return_value();
    }
  }

  bool done() const { return !_handle || _handle.done(); }

  awaitable operator co_await() const noexcept {
    awaitable awaiter;
    awaiter._awaiter = _handle;
    return awaiter;
  }

private:
  handle_type _handle;
};
} // namespace bt