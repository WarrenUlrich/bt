#pragma once

#include <bt/behavior_node.hpp>

namespace bt {
template <typename Context, BehaviorNode<Context> Node>
class retry_node {
public:
  constexpr retry_node(std::size_t count, Node &&node)
      : _node(std::move(node)), _count(count) {}
  
  node_task tick(Context &context) {
    auto task = _node.tick(context);
    
    for (std::size_t i = 0; i < _count; ++i) {
      while (true) {
        auto status = task.tick();
        if (status == node_status::running) {
          co_yield node_status::running;
        } else {
          if (status == node_status::success)
            co_return node_status::success;
          else if (i < _count - 1)
            co_yield node_status::running;
            
          break;
        }
      }
    }

    co_return node_status::failure;
  }

private:
  std::size_t _count; 
  Node _node;
};
} // namespace bt