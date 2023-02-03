#pragma once

#include <bt/behavior_node.hpp>

namespace bt {
template <typename Context, BehaviorNode<Context> Node>
class repeat_node : public Node {
public:
  template <typename... Args>
  constexpr repeat_node(std::size_t count, Args &&...args)
      : Node(std::forward<Args>(args)...), _count(count) {}

  node_task tick(Context &context) {
    for (std::size_t i = 0; i < _count; ++i) {
      auto task = Node::tick(context);
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
};

template <typename Context, BehaviorNode<Context> Node, typename... Args>
repeat_node<Context, Node> repeat(std::size_t count, Args &&...args) {
  return repeat_node<Context, Node>(count, std::forward<Args>(args)...);
}
} // namespace bt