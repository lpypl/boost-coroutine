#include <boost/coroutine/all.hpp>
#include <iostream>
#include <thread>

typedef boost::coroutines::asymmetric_coroutine<int> asymmetric_coroutine;

int main(int argc, char const *argv[])
{
    asymmetric_coroutine::push_type writer(
        [&](asymmetric_coroutine::pull_type &datasource) {
            std::cout << "[Writer<" << std::this_thread::get_id() << ">] " << "" << std::endl;
            while(datasource) {
                std::cout << "[Writer<" << std::this_thread::get_id() << ">] " << datasource.get() << std::endl;
                datasource();
            }
        }
    );
    
    std::cout << "[Datasource<" << std::this_thread::get_id() << ">] " << "" << std::endl;
    for (auto it = boost::coroutines::begin(writer); it != boost::coroutines::end(writer); ++it) {
        int data = rand();
        std::cout << "[Datasource<" << std::this_thread::get_id() << ">] " << data << std::endl;
        *it = data;
    }
    return 0;
}
