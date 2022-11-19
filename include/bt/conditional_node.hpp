#pragma once

#include <bt/status.hpp>

namespace bt {

  template<typename Context, typename Identifier, typename Predicate>
  class conditional_node;

  template<typename Context, typename Predicate>
  class conditional_node<Context, void, Predicate> {
  public:
    constexpr conditional_node(const Predicate &predicate) : _predicate(predicate) {}

    constexpr conditional_node(Predicate &&predicate) : _predicate(std::move(predicate)) {}

    status tick(Context &context) const {
      return _predicate(context) ? status::success : status::failure;
    }
    
  private:
    Predicate _predicate;
  };
  
  template<typename Context, typename Identifier, typename Predicate>
  class conditional_node {
  public:
    constexpr conditional_node(const Identifier &id, const Predicate &predicate) : _id(id), _predicate(predicate) {}

    constexpr conditional_node(Identifier &&id, Predicate &&predicate) : _id(std::move(id)), _predicate(std::move(predicate)) {}

    status tick(Context &context) const {
      return _predicate(context) ? status::success : status::failure;
    }

    friend std::ostream &operator<<(std::ostream &os, const conditional_node &node) {
      os << "conditional<" << node._id << ">";
      return os;
    }
  
  private:
    Identifier _id;
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

  template<typename Context, typename Identifier, typename Predicate>
  constexpr conditional_node<Context, Identifier, Predicate> make_conditional_node(const Identifier &id, const Predicate &predicate) {
    return conditional_node<Context, Identifier, Predicate>(id, predicate);
  }

  template<typename Context, typename Identifier, typename Predicate>
  constexpr conditional_node<Context, Identifier, Predicate> make_conditional_node(Identifier &&id, Predicate &&predicate) {
    return conditional_node<Context, Identifier, Predicate>(std::move(id), std::move(predicate));
  }
}