/*
这里在主线程里面创建了纤程，还看到一种用法是在单独线程里面创建一些纤程，
好像就不必使主程序等待纤程了，只要等待子线程？
*/

#include <boost/fiber/all.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <thread>

const int FIBERS_MAX = 100000; 

int main() {
    boost::fibers::fiber *mfibers[FIBERS_MAX];

    for (int i=0; i<FIBERS_MAX; i++) {
        mfibers[i] = new boost::fibers::fiber([i]() {
            for (int j=0; j<3; j++) {
                std::cout << boost::format{"[Fiber<%1%, %2%>] %3%"} % i % std::this_thread::get_id() % j << std::endl;
                boost::this_fiber::yield();
            }
        });
        // mfibers[i]->detach();
    }
    while (true) {
        std::cout << boost::format{"[Main<%1%, %2%>] %3%"} % "su" % std::this_thread::get_id() % "Main" << std::endl;
        if (mfibers[0]->joinable()) {
            mfibers[0]->join();
        } else {
            std::cout << boost::format{"[Main<%1%, %2%>] %3%"} % "su" % std::this_thread::get_id() % "Over" << std::endl;
            break;
        }
    }
}