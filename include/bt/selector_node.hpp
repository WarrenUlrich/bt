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

  status tick(Context &context) {
    status result = status::running;

    meta::visit_at(
        [&](auto &node) {
          result = node.tick(context);
          switch (result) {
          case status::success:
            _current_node_index = 0;
            break;
          case status::failure:
            ++_current_node_index;
            if (_current_node_index >= std::tuple_size_v<decltype(_nodes)>) {
              _current_node_index = 0;
              result = status::failure;
            }
            break;
          }
        },
        _nodes, _current_node_index);
    return result;
  }

  void display(std::ostream &os, int indent = 0) const {
    os << "sequence_node\n";
    auto for_all = [&](auto &&f) {
      std::apply([&](auto &&...nodes) { (f(nodes), ...); }, _nodes);
    };

    std::size_t idx = 0;
    for_all([&](const auto &node) { 
      if (idx == std::tuple_size_v<decltype(_nodes)> - 1)
        os << std::string(indent + 2, ' ') << "`--";
      else
        os << std::string(indent + 2, ' ') << "|--";
      if constexpr (requires { node.display(os, indent + 4); }) { // display
        node.display(os, indent + 4);
      } else { // ostream
        os << node << '\n';
      }
      ++idx;
    });
  }

  friend std::ostream &operator<<(std::ostream &os, const selector_node &node) {
    node.display(os);
    return os;
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