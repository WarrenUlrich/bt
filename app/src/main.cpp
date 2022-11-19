#include <bt/sequence_node.hpp>
#include <bt/action_node.hpp>
#include <bt/selector_node.hpp>
#include <bt/conditional_node.hpp>
#include <iostream>
#include <string_view>

auto bhv = bt::make_sequence_node<int>(
  bt::make_action_node<int, std::string_view>("Action1", [](auto& ctx) {
    std::cout << "Action1" << std::endl;
    return bt::status::success;
  }),
  bt::make_action_node<int, std::string_view>("Action2", [](auto& ctx) {
    std::cout << "Action2" << std::endl;
    return bt::status::success;
  }),
  // new sequence node
  bt::make_sequence_node<int>(
    bt::make_action_node<int, std::string_view>("Action3", [](auto& ctx) {
      std::cout << "Action3" << std::endl;
      return bt::status::success;
    }),
    bt::make_action_node<int, std::string_view>("Action4", [](auto& ctx) {
      std::cout << "Action4" << std::endl;
      return bt::status::success;
    }),
    bt::make_sequence_node<int>(
      bt::make_action_node<int, std::string_view>("Action5", [](auto& ctx) {
        std::cout << "Action5" << std::endl;
        return bt::status::success;
      }),
      bt::make_action_node<int, std::string_view>("Action6", [](auto& ctx) {
        std::cout << "Action6" << std::endl;
        return bt::status::success;
      }),
      bt::make_conditional_node<int, std::string_view>("Conditional1", [](auto& ctx) {
        return true;
      })
    )
  )
);

int main(int argc, char** args) {
  int ctx = 0;
  bhv.tick(ctx);

  std::cout << bhv << std::endl;
  return 0;
}