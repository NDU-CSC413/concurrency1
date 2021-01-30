/* Note on compiler optimizations
* EXAMPLE1: call std::any_of without using the result
* EXAMPLE2: call std::any_of and using the result
* EXAMPLE3: call our implementation of any_of without using the result
* EXAMPLE4: call std::for_each to modify the values of a vector
*/

#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
#include "utitlities.h"
#define EXAMPLE4



#ifdef EXAMPLE3
template<typename Iter,typename F>
bool any_of(Iter start, Iter end, F f) {
    Iter itr = start;
    while (itr != end) {
        if (f(*itr)) return true;
        ++itr;
    }
    return false;
}
#endif 

int main()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 1000);
    const int n = 100000000;
    std::vector<int> v(n);
    std::generate(v.begin(), v.end(), [&]() {return dist(rd); });
    auto start = std::chrono::high_resolution_clock::now();
#if defined(EXAMPLE1)|| defined(EXAMPLE2)
    bool result=std::any_of(v.begin(), v.end(), [](int x) {return x == 2000; });
#elif defined(EXAMPLE3)
    bool result = ::any_of(v.begin(), v.end(), [](int x) {return x == 2000; });
#elif defined(EXAMPLE4)
    std::for_each(v.begin(), v.end(), [](int& x) {x *= 2; });
#else
#error "Please choose an example"
#endif 
    auto end = std::chrono::high_resolution_clock::now();
    print_duration(start, end,"");
#if defined(EXAMPLE2)
    std::cout << " result = " << std::boolalpha << result << std::endl;
#endif 
}

