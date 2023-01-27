#pragma once

#include <bt/behavior_node.hpp>

namespace bt {
template <typename Context, BehaviorNode<Context> Node>
class repeat_node : public Node {
public:
  using context_type = Context;
  
  template <typename... Args>
  constexpr repeat_node(std::size_t count, const Args &...args)
      : Node(args...), _count(count) {}

  template <typename... Args>
  constexpr repeat_node(std::size_t count, Args &&...args)
      : Node(std::forward<Args>(args)...), _count(count) {}

  node_status tick(Node::context_type &ctx) {
    node_status result = node_status::running;

    if (_current_count < _count) {
      result = Node::tick(ctx);
      switch (result) {
      case node_status::success:
        ++_current_count;
        break;
      case node_status::failure:
        _current_count = 0;
        break;
      }
    } else {
      _current_count = 0;
      result = node_status::success;
    }
    return result;
  }

private:
  std::size_t _count;
  std::size_t _current_count = 0;
};
} // namespace bt