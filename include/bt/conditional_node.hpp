#include <bt/behavior_node.hpp>

namespace bt {

template <typename C, typename Context>
concept Condition = requires(Context context, C condition) {
                      { condition(context) } -> std::same_as<bool>;
                    };

template <typename Context, Condition<Context> Condition>
class conditional_node {
public:
  constexpr conditional_node(Condition &&condition)
      : _condition(std::move(condition)) {}

  node_task tick(Context &context) {
    if (_condition(context)) {
      co_return node_status::success;
    } else {
      co_return node_status::failure;
    }
  }

private:
  Condition _condition;
};
} // namespace bt