#pragma once

#include <bt/status.hpp>

namespace bt {
  template<typename Context, typename Predicate>
  class conditional_node {
  public:
    constexpr conditional_node(const Predicate &predicate) : _predicate(predicate) {}

    constexpr conditional_node(Predicate &&predicate) : _predicate(std::move(predicate)) {}

    status tick(Context &context) const {
      return _predicate(context) ? status::success : status::failure;
    }
    
  private:
    Predicate _predicate;
  };

  template<typename Context, typename Predicate>
  constexpr conditional_node<Context, Predicate> make_conditional_node(const Predicate &predicate) {
    return conditional_node<Context, Predicate>(predicate);
  }

  template<typename Context, typename Predicate>
  constexpr conditional_node<Context, Predicate> make_conditional_node(Predicate &&predicate) {
    return conditional_node<Context, Predicate>(std::move(predicate));
  }
}