

#include <iostream>
#include <exception>
#include <thread>


/* Examples to illustrate handling
* of exceptions when using threads
*/

#define EXAMPLE2


/* This functions starts a thread
* then before joining the thread
* it calls a function that throws 
*/
#ifdef EXAMPLE1
/* a function that does nothing */
void threadf() {}

/* a function that throws an exceptions*/
void throwf() {
    std::cout << "starting throwf\n";
    throw std::exception{  };
}
void runt() {
    std::thread t(threadf);
    throwf();
    t.join();
}
int main()
{

    runt();
    std::cout << "main thread is done\n";
}

#endif 

#if defined(EXAMPLE2) || defined(EXAMPLE3)
void threadf() {}

/* a function that throws an exceptions*/
void throwf() {
    std::cout << "starting throwf\n";
    throw std::exception{  };
}

struct thread_guard {
    std::thread& _t;

    thread_guard(std::thread& t) :_t(t) { }
    ~thread_guard() {
        if (_t.joinable())
            _t.join();
        std::cout << "thead guard dtor\n";
}
};
#endif 
/* The same function but using
* a thread guard 
*/
#ifdef EXAMPLE2
void runt() {
    std::thread t(threadf);
    thread_guard g(t);
    throwf();
}

int main()
{

    runt();
    std::cout << "main thread is done\n";
}
#endif 
#ifdef EXAMPLE3
void runt() {
    std::thread t(throwf);
    thread_guard g(t);
}
#endif 
#ifdef EXAMPLE4
/* a function that does nothing */
void threadf() {}

/* a function that throws an exceptions*/
void throwf() {
    std::cout << "starting throwf\n";
    throw std::exception{  };
}

/* a function wrapper that
* calls another function
* that might throw
*/
template<typename F>
void runt(F f) {
    try {
        f();
    }
    catch (...) {
    }
}
#endif 

//
//int main()
//{
//    
//#ifndef EXAMPLE4
//    try {
//        runt();
//    }
//    catch (...) {}
//#else
//    runt(throwf);
//     std::thread t(runt<decltype(throwf)>, throwf);
//     t.join();
//#endif 
//    std::cout << "main thread is done\n";
//}
