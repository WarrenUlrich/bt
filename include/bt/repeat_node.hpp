#pragma once

#include <bt/behavior_node.hpp>

namespace bt {
template <typename Context, BehaviorNode<Context> Node> 
class repeat_node {
public:
  constexpr repeat_node(std::size_t count, Node &&node)
      : _node(std::move(node)), _count(count) {}

  node_task tick(Context &context) {
    for (std::size_t i = 0; i < _count; ++i) {
      auto task = _node.tick(context);
      while (true) {
        auto status = task.tick();
        if (status == node_status::running) {
          co_yield node_status::running;
        } else {
          if (status == node_status::failure)
            co_return node_status::failure;
          else if (i < _count - 1)
            co_yield node_status::running;

          break;
        }
      }
    }

    co_return node_status::success;
  }

private:
  std::size_t _count;
  Node _node;
};

template <typename Context, BehaviorNode<Context> Node>
repeat_node<Context, Node> repeat(std::size_t count, Node &&node) {
  return repeat_node<Context, Node>(count, std::move(node));
}
} // namespace bt