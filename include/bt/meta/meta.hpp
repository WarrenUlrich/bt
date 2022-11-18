#pragma once

namespace bt::meta {
  template<std::size_t I = 0>
  void visit_at(auto&& func, auto& tuple, std::size_t idx) {
      if (I == idx) {
          func(std::get<I>(tuple));
          return;
      }

      if constexpr (I + 1 < std::tuple_size_v<std::decay_t<decltype(tuple)>>) {
          return visit_at<I + 1>(func, tuple, idx);
      }
  }  
}