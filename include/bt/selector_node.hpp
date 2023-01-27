#pragma once

#include <vector>
#include <memory>
#include <concepts>

#include <bt/behavior_node.hpp>
#include <bt/meta/meta.hpp>

namespace bt {
template <typename Context, BehaviorNode<Context>... Nodes>
class selector_node {
public:
  constexpr selector_node(const Nodes &...nodes)
      : _nodes(std::make_tuple(nodes...)) {}

  constexpr selector_node(Nodes &&...nodes)
      : _nodes(std::make_tuple(std::move(nodes)...)) {}

  node_status tick(Context &context) {
    node_status result = node_status::running;

    meta::visit_at(
        [&](auto &node) {
          result = node.tick(context);
          switch (result) {
          case node_status::success:
            _current_node_index = 0;
            break;
          case node_status::failure:
            ++_current_node_index;
            if (_current_node_index >= std::tuple_size_v<decltype(_nodes)>) {
              _current_node_index = 0;
              result = node_status::failure;
            }
            break;
          }
        },
        _nodes, _current_node_index);
    return result;
  }

private:
  std::tuple<Nodes...> _nodes;
  std::size_t _current_node_index = 0;
};

template <typename Context, BehaviorNode<Context>... Nodes>
constexpr selector_node<Context, Nodes...>
make_selector_node(const Nodes &...nodes) {
  return selector_node<Context, Nodes...>(nodes...);
}

template <typename Context, BehaviorNode<Context>... Nodes>
constexpr selector_node<Context, Nodes...>
make_selector_node(Nodes &&...nodes) {
  return selector_node<Context, Nodes...>(std::move(nodes)...);
}
} // namespace bt