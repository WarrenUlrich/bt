#include <iostream>
#include <coroutine>
#include <tuple>
#include <utility>
#include <algorithm>
#include <array>

#include <bt/bt.hpp>

class custom_node {
public:
  bt::node_task tick(int &context) const {
    std::cout << "tick1\n";
    co_yield bt::node_status::running;
    std::cout << "tick2\n";
    co_return bt::node_status::success;
  }
};

class custom_node2 {
public:
  bt::node_task tick(int &context) const {
    std::cout << "tick3\n";
    co_yield bt::node_status::running;
    std::cout << "tick4\n";
    co_return bt::node_status::success;
  }
};

int main(int argc, char **args) {
  auto seq = bt::sequence<int>(
    custom_node(),
    bt::repeat<int, custom_node2>(2)
  );

  int context = 0;
  auto task = seq.tick(context);

  bt::node_status status;
  while (!task.done()) {
    status = task.tick();
    std::cout << "status " << (int)status << std::endl;
  }

  // std::cout << "status " << (int)status << std::endl;
  std::cout << "Done" << std::endl;

  return 0;
}