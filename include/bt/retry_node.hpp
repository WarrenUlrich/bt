#pragma once

#include <bt/behavior_node.hpp>

namespace bt {
  template<typename Context, BehaviorNode<Context> Node>
  class retry_node {
  public:
    constexpr retry_node(const Node &node, std::size_t count) : _node(node), _count(count) {}

    constexpr retry_node(Node &&node, std::size_t count) : _node(std::move(node)), _count(count) {}

    status tick(Context &context) {
      status result = status::running;

      if (_current_count < _count) {
        result = _node.tick(context);
        switch (result) {
        case status::success:
          _current_count = 0;
          break;
        case status::failure:
          ++_current_count;
          break;
        }
      } else {
        _current_count = 0;
        result = status::failure;
      }
      return result;
    }

  private:
    std::size_t _count;
    std::size_t _current_count = 0;
    Node _node;
  };

  template<typename Context, BehaviorNode<Context> Node>
  constexpr retry_node<Context, Node> make_retry_node(const Node &node, std::size_t count) {
    return retry_node<Context, Node>(node, count);
  }

  template<typename Context, BehaviorNode<Context> Node>
  constexpr retry_node<Context, Node> make_retry_node(Node &&node, std::size_t count) {
    return retry_node<Context, Node>(std::move(node), count);
  }
}