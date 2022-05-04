#include <boost/coroutine/all.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

std::vector<int> merge(const std::vector<int> &a, const std::vector<int> &b)
{
    std::vector<int> c;
    std::size_t idx_a = 0, idx_b = 0;
    boost::coroutines::symmetric_coroutine<void>::call_type *other_a = 0, *other_b = 0;

    boost::coroutines::symmetric_coroutine<void>::call_type coro_a(
        [&](boost::coroutines::symmetric_coroutine<void>::yield_type &yield)
        {
            while (idx_a < a.size())
            {
                if (b[idx_b] < a[idx_a]) // test if element in array b is less than in array a
                    yield(*other_b);     // yield to coroutine coro_b
                c.push_back(a[idx_a++]); // add element to final array
            }
            // add remaining elements of array b
            while (idx_b < b.size())
                c.push_back(b[idx_b++]);
        });

    boost::coroutines::symmetric_coroutine<void>::call_type coro_b(
        [&](boost::coroutines::symmetric_coroutine<void>::yield_type &yield)
        {
            while (idx_b < b.size())
            {
                if (a[idx_a] < b[idx_b]) // test if element in array a is less than in array b
                    yield(*other_a);     // yield to coroutine coro_a
                c.push_back(b[idx_b++]); // add element to final array
            }
            // add remaining elements of array a
            while (idx_a < a.size())
                c.push_back(a[idx_a++]);
        });

    other_a = &coro_a;
    other_b = &coro_b;

    coro_a(); // enter coroutine-fn of coro_a

    return c;
}

template <class T> void print(std::vector<T> vec) {
    std::for_each(std::begin(vec), std::end(vec), [](T t) {
        std::cout << t << " ";
    });
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    std::vector<int> a = {1, 5, 6, 10};
    std::vector<int> b = {2, 4, 7, 8, 9, 13};
    std::vector<int> c = merge(a, b);
    print(a);
    print(b);
    print(c);
    return 0;
}
