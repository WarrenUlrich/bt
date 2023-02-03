#pragma once

#include <vector>
#include <memory>
#include <concepts>

#include <bt/behavior_node.hpp>

namespace bt {
template <typename Context, BehaviorNode<Context>... Nodes>
class selector_node {
public:
  constexpr selector_node(Nodes &&...nodes) : _nodes(std::move(nodes)...) {}

  template <std::size_t I = 0> node_task tick(Context &context) {
    if constexpr (I < sizeof...(Nodes)) {
      auto &node = std::get<I>(_nodes);
      auto task = node.tick(context);

      while (true) {
        auto status = task.tick();
        switch (status) {
        case node_status::success:
          co_return node_status::success;
        case node_status::failure: {
          if constexpr (I < sizeof...(Nodes) - 1)
            co_yield node_status::running;
          else
            co_return node_status::failure;

          auto next_task = tick<I + 1>(context);
          while (true) {
            auto next_status = next_task.tick();
            if (next_status == node_status::running) {
              co_yield node_status::running;
            } else {
              co_return next_status;
            }
          }
        }
        case node_status::running:
          co_yield node_status::running;
        }
      }
    } else {
      co_return node_status::failure;
    }
  }

private:
  std::tuple<Nodes...> _nodes;
};

template <typename Context, BehaviorNode<Context>... Nodes>
selector_node<Context, Nodes...> selector(Nodes &&...nodes) {
  return selector_node<Context, Nodes...>(std::move(nodes)...);
}
} // namespace bt