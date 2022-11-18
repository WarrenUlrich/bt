#pragma once

#include <bt/status.hpp>

namespace bt {
template <typename Node, typename Context>
concept BehaviorNode = requires(Node n, Context ctx) {
  { n.tick(ctx) } -> std::same_as<status>;
};
} // namespace bt