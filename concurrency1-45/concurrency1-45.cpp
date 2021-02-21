/*****************************************************************//**
 * @file   concurrency1-45.cpp
 * @brief  left and right fold
 * 
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/
#include <iostream>
#include <numeric>
#include <vector>
template<typename Iter, typename T, typename Func = std::plus<T> >
T my_accumulate(Iter start, Iter end, T init, Func f = std::plus<T>{}) {
    T result = init;
    for (auto itr = start; itr != end; ++itr) {
        result =result+*itr;
    }
    return result;
}
template<typename T, typename ...Ts>
auto foldr(Ts... params) {
    return (params + ... + T{});
}
template<typename T, typename ...Ts>
auto foldl(Ts... params) {
    return (T{} + ... + params);
}
struct mid {
    int _x;
    mid(int x=0) :_x(x) {}
    mid operator+(const mid& rhs) {
        return mid((_x + rhs._x) / 2);
    }

};
int main() {
    std::vector<int> u{ 1,2,3,4 };
    std::accumulate(u.begin(), u.end(), 0, std::plus{});
    mid a{ 1 }, b{ 2 }, c{ 3 };
    mid rr = foldr<mid>(a, b, c);
    mid lr = foldl<mid>(a, b, c);
    std::cout << rr._x << "\n";
    std::cout << lr._x << "\n";
    std::vector<mid> v{ a,b,c };
    auto ar = std::accumulate(v.begin(), v.end(),mid{});
    std::cout<<my_accumulate(v.begin(), v.end(), mid{})._x<<"\n";
    std::cout << ar._x << "\n";
}