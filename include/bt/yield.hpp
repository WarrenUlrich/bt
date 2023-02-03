#pragma once

#include <bt/node_status.hpp>

namespace bt {
class yield {
public:
  constexpr yield() noexcept = default;

  constexpr bool await_ready() const noexcept { return false; }

  constexpr bool await_suspend(std::coroutine_handle<> handle) noexcept {
    return true;
  }

  constexpr bt::node_status await_resume() const noexcept {
    return bt::node_status::running;
  }
};
} // namespace bt