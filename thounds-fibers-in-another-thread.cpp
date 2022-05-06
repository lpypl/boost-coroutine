#include <boost/fiber/all.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <thread>

const int FIBERS_MAX = 100000;

int main() {
  // 创建一个Thread，用于执行Fibers
  std::thread fiber_executor([]() {
    boost::fibers::fiber *mfibers[FIBERS_MAX];

    for (int i = 0; i < FIBERS_MAX; i++) {
      mfibers[i] = new boost::fibers::fiber([i]() {
        for (int j = 0; j < 3; j++) {
          std::cout << boost::format{"[Fiber<%1%, %2%>] %3%"} % i %
                           std::this_thread::get_id() % j
                    << std::endl;
          boost::this_fiber::yield();
        }
      });
      // mfibers[i]->detach();
    }
  });
  std::cout << boost::format{"[Main<%1%, %2%>] %3%"} % "main" %
                   std::this_thread::get_id() % "Main"
            << std::endl;
  fiber_executor.join();
  std::cout << boost::format{"[Main<%1%, %2%>] %3%"} % "main" %
                   std::this_thread::get_id() % "Over"
            << std::endl;
}