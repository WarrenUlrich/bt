#pragma once

#include <bt/status.hpp>

namespace bt {
  template<typename Func, typename Context>
  concept Action = requires(Func f, Context ctx) {
    { f(ctx) } -> std::same_as<status>;
  };

  template<typename Context, typename Identifier, Action<Context> Func>
  class action_node;

  template<typename Context, Action<Context> Func>
  class action_node<Context, void, Func> {
  public:
    constexpr action_node(const Func &func) : _func(func) {}

    constexpr action_node(Func &&func) : _func(std::move(func)) {}

    status tick(Context &context) const {
      return _func(context);
    }

  private:
    Func _func;
  };

  template<typename Context, typename Identifier, Action<Context> Func>
  class action_node { 
  public:
    constexpr action_node(const Identifier &id, const Func &func) : _id(id), _func(func) {}

    constexpr action_node(Identifier &&id, Func &&func) : _id(std::move(id)), _func(std::move(func)) {}

    status tick(Context &context) const {
      return _func(context);
    }

    friend std::ostream &operator<<(std::ostream &os, const action_node &node) {
      os << "action<" << node._id << ">";
      return os;
    }

  private:
    Identifier _id;
    Func _func;
  };

  template<typename Context, Action<Context> Func>
  constexpr action_node<Context, void, Func> make_action_node(const Func &func) {
    return action_node<Context, void, Func>(func);
  }

  template<typename Context, Action<Context> Func>
  constexpr action_node<Context, void, Func> make_action_node(Func &&func) {
    return action_node<Context, void, Func>(std::move(func));
  }

  template<typename Context, typename Identifier, Action<Context> Func>
  constexpr action_node<Context, Identifier, Func> make_action_node(const Identifier &id, const Func &func) {
    return action_node<Context, Identifier, Func>(id, func);
  }

  template<typename Context, typename Identifier, Action<Context> Func>
  constexpr action_node<Context, Identifier, Func> make_action_node(Identifier &&id, Func &&func) {
    return action_node<Context, Identifier, Func>(std::move(id), std::move(func));
  }
}