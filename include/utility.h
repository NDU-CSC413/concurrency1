#pragma once
#include <chrono>
#include <string>
std::string fail(std::string s){
   return    "\x1B[31m"+s+"\033[0m";
}
std::string success(std::string s){
   return    "\x1B[32m"+s+"\033[0m";
}
std::string header(std::string s){
   return "\x1B[33m"+s+"\033[0m"+"\n-----------------\n";
}

using Duration = std::chrono::duration<double, std::milli>;

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(end - start);\
}

