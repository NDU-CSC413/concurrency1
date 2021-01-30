#pragma once
#include <iostream>
#include <string>
template<typename T>
void print_duration(T start, T end,std::string s) {
    auto duration = std::chrono::duration<double, std::ratio<1, 1000>>(end - start);
    std::cout << "\nduration "<<s <<" "<< duration.count() << "\n";
}
