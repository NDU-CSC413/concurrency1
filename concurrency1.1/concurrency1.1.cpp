

#include <iostream>
#include <exception>
#include <thread>

#define EXAMPLE2

void myf() {
    throw std::exception{ "exception" };
}

#ifdef EXAMPLE1
void doit() {
    std::thread t{ []() {return 7; } };
    myf();
    t.join();
}
#endif 


#ifdef EXAMPLE2
class thread_guard {
    std::thread& _t;
public:
    thread_guard(std::thread& t) :_t(t) {}

    ~thread_guard() {
        if (_t.joinable())_t.join();
    }
        
};
void doit() {
    std::thread t{ []() {return 7; } };
    thread_guard g(t);
    myf();
//    t.join();
}

#endif

int main()
{
    try {
        doit();
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
    std::cout << "Hello World!\n";
}
