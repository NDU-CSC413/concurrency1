// concurrency1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

class bg_task {
    int x;
public:
    bg_task(int v) :x(v) {}
    void operator()() {
        std::cout << "value of x is " << x << "\n";
    }

};

// every thread has an "entry" function
void hello() {
    std::cout << "hello concurrency\n";
}

int f1() {
    return 7;
}
int f2(int (*)());

int f2(int (*f)()) {
    return (*f)();
}
#define EXAMPLE4

int main() {

#ifdef EXAMPLE1
    std::cout << hello << "\n";
    std::thread t1(hello);
    t1.join();
    std::cout << f2(f1);
#endif 

#ifdef EXAMPLE2
    bg_task bgt(7);
    std::thread t2(bgt);
    t2.join();
#endif 

#ifdef EXAMPLE3
    std::thread t3([]() {
        std::cout << "from lambda\n";
        }
    );
    t3.join();

#endif 

#ifdef EXAMPLE4
   
    std::thread t3([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::ofstream file;
        file.open("output.txt");
        file<< "from lambda\n";
        }
    );
    t3.join();

#endif
}