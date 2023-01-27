#pragma once

#include <bt/node_status.hpp>

namespace bt {
  template<typename Context, typename Predicate>
  class conditional_node {
  public:
    using context_type = Context;
    
    constexpr conditional_node(const Predicate &predicate) : _predicate(predicate) {}

    constexpr conditional_node(Predicate &&predicate) : _predicate(std::move(predicate)) {}

    node_status tick(Context &context) const {
      return _predicate(context) ? node_status::success : node_status::failure;
    }
    
  private:
    Predicate _predicate;
  };
  
  template<typename Context, typename Predicate>
  constexpr conditional_node make_conditional_node(const Predicate &predicate) {
    return conditional_node<Context, Predicate>(predicate);
  }

  template<typename Context, typename Predicate>
  constexpr conditional_node make_conditional_node(Predicate &&predicate) {
    return conditional_node<Context, Predicate>(std::move(predicate));
  }
}