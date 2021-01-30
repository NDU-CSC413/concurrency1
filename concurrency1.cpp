#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

struct function_object {
    int x;
    function_object(int v) :x(v) {}
    void operator()() {
        std::cout << "value of x is " << x << "\n";
    }

};

// every thread has an "entry" function
void hello() {
    std::cout << "hello concurrency\n";
}


int main() {
    //Using a function
    std::thread t1(hello);

    //Using a function object
    function_object bgt{ 7 };
    std::thread t2(bgt);
    //Using a lambda
    std::thread t3([]() {
        std::cout << "from lambda\n";
        }
    );
   
    t1.join();
    t2.join();
    t3.join();
    

}