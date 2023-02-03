#pragma once

#include <bt/behavior_node.hpp>
#include <bt/yield.hpp>

namespace bt {
template <typename Context, BehaviorNode<Context>... Nodes>
class parallel_node {
public:
  constexpr parallel_node(Nodes &&...nodes) : _nodes(std::move(nodes)...) {}

  node_task tick(Context &context) {
    auto tasks = _get_tasks(context);

    while (true) {
      for (std::size_t i = 0; i < sizeof...(Nodes); ++i) {
        auto &task = tasks[i];
        if (task.done())
          continue;

        auto status = task.tick();
        switch (status) {
        case node_status::failure:
          co_return node_status::failure;
        case node_status::running:
          co_await yield();
        }
      }

      if (std::all_of(tasks.cbegin(), tasks.cend(),
                      [](auto &task) { return task.done(); }))
        co_return node_status::success;
    }

    co_return node_status::success;
  }

private:
  std::tuple<Nodes...> _nodes;

  template <std::size_t I = 0>
  void _add_tasks_recursive(auto &arr, Context &ctx) {
    if constexpr (I < sizeof...(Nodes)) {
      arr[I] = std::get<I>(_nodes).tick(ctx);
      _add_tasks_recursive<I + 1>(arr, ctx);
    }
  }

  std::array<node_task, sizeof...(Nodes)> _get_tasks(Context &ctx) {
    std::array<node_task, sizeof...(Nodes)> tasks;
    _add_tasks_recursive(tasks, ctx);
    return tasks;
  }
};

template <typename Context, BehaviorNode<Context>... Nodes>
parallel_node<Context, Nodes...> parallel(Nodes &&...nodes) {
  return parallel_node<Context, Nodes...>(std::move(nodes)...);
}
} // namespace bt