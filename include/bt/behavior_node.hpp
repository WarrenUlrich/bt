#pragma once

#include <bt/node_status.hpp>

namespace bt {
template <typename Node, typename Context>
concept BehaviorNode = requires(Node n, Context ctx) {
  { Node::context_type } -> std::same_as<Context>;
  { n.tick(ctx) } -> std::same_as<node_status>;
};
} // namespace bt