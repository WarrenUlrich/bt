#pragma once

#include <bt/node_task.hpp>

namespace bt {
template <typename Node, typename Context>
concept BehaviorNode = requires(Context context, Node node) {
                         { node.tick(context) } -> std::same_as<node_task>;
                       };
} // namespace bt