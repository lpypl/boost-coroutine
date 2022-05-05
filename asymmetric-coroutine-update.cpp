#include <boost/coroutine/all.hpp>
#include <iostream>
#include <thread>

typedef boost::coroutines::asymmetric_coroutine<int> asymmetric_coroutine;

int main(int argc, char const *argv[])
{
    asymmetric_coroutine::pull_type source(
        [&](asymmetric_coroutine::push_type &sink) {
            std::cout << "[Source<" << std::this_thread::get_id() << ">] " << "" << std::endl;
            int first=1, second=1;
            sink(first);
            sink(second);
            for (int i=0; i<8; i++) {
                int third = first + second;
                first = second;
                second = third;
                sink(third);
            }
        }
    );
    
    std::cout << "[Sink<" << std::this_thread::get_id() << ">] " << "" << std::endl;
    for (auto it = boost::coroutines::begin(source); it != boost::coroutines::end(source); ++it) {
        std::cout << *it << " " << std::flush;
    }
    std :: cout << "\nDone " << std :: endl ;
    return 0;
}
