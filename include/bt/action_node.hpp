#include <bt/behavior_node.hpp>

namespace bt {
template <typename A, typename Context>
concept Action = requires(Context context, A action) {
                   { action(context) } -> std::same_as<node_task>;
                 };

template <typename Context, Action<Context> Action>
class action_node {
public:
  constexpr action_node(Action&& action) : _action(std::move(action)) {}

  node_task tick(Context &context) {
    auto task = _action(context);
    while (true) {
      auto status = task.tick();
      if (status == node_status::running) {
        co_yield node_status::running;
      } else {
        co_return status;
      }
    }
  }

private:
  Action _action;
};

template <typename Context, Action<Context> Action>
action_node<Context, Action> action(Action&& action) {
  return action_node<Context, Action>(std::move(action));
}

} // namespace bt