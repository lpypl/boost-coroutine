#include <boost/coroutine/all.hpp>
#include <iostream>
#include <vector>
#include <queue>

void elegant()
{
    int capacity = 3;
    std::queue<int> store;

    boost::coroutines::symmetric_coroutine<void>::call_type *other_a = 0, *other_b = 0;

    boost::coroutines::symmetric_coroutine<void>::call_type coro_a(
        [&](boost::coroutines::symmetric_coroutine<void>::yield_type &yield)
        {
            while (true)
            {
                while (store.size() < capacity)
                {
                    int product = rand();
                    store.push(product);
                    std::cout << "[Producer] " << product << std::endl;
                }
                yield(*other_b);
            }
        });

    boost::coroutines::symmetric_coroutine<void>::call_type coro_b(
        [&](boost::coroutines::symmetric_coroutine<void>::yield_type &yield)
        {
            while (true)
            {
                while (store.size() > 0)
                {
                    int product = store.back();
                    store.pop();
                    std::cout << "[Consumer] " << product << std::endl;
                }
                yield(*other_a);
            }
        });

    other_a = &coro_a;
    other_b = &coro_b;

    coro_a(); // enter coroutine-fn of coro_a
}

int main(int argc, char const *argv[])
{
    elegant();
    return 0;
}
