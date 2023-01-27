#pragma once

#include <bt/node_status.hpp>

namespace bt {
  template<typename Func, typename Context>
  concept Action = requires(Func f, Context ctx) {
    { f(ctx) } -> std::same_as<node_status>;
  };

  template<typename Context, Action<Context> Func>
  class action_node {
  public:
    using context_type = Context;
    
    constexpr action_node(const Func &func) : _func(func) {}

    constexpr action_node(Func &&func) : _func(std::move(func)) {}

    node_status tick(Context &context) const {
      return _func(context);
    }

  private:
    Func _func;
  };

  template<typename Context, Action<Context> Func>
  constexpr action_node<Context, Func> make_action_node(const Func &func) {
    return action_node<Context, Func>(func);
  }

  template<typename Context, Action<Context> Func>
  constexpr action_node<Context, Func> make_action_node(Func &&func) {
    return action_node<Context, Func>(std::move(func));
  }

}