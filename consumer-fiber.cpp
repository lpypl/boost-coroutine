//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <boost/intrusive_ptr.hpp>
#include <boost/fiber/all.hpp>
#include <queue>

int capacity = 3;
std::queue<int> store;

inline void produce()
{
    while (true)
    {
        while (store.size() < capacity)
        {
            int product = rand();
            store.push(product);
            std::cout << "[Producer] " << product << std::endl;
        }
        boost::this_fiber::yield();
    }
}

inline void consume()
{
    while (true)
    {
        while (store.size() > 0)
        {
            int product = store.back();
            store.pop();
            std::cout << "[Consumer] " << product << std::endl;
        }
        boost::this_fiber::yield();
    }
}

int main()
{
    try
    {
        boost::fibers::fiber producer_fiber(produce);
        boost::fibers::fiber consumer_fiber(consume);
        producer_fiber.join();
        consumer_fiber.join();
        std::cout << "done." << std::endl;

        return EXIT_SUCCESS;
    }
    catch (std::exception const &e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unhandled exception" << std::endl;
    }
    return EXIT_FAILURE;
}