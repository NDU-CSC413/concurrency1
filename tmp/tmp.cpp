// tmp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include <algorithm>
template<typename V,typename ...Types>
struct tuple {

};
template<typename T,typename ...Ts>
auto doit(Ts... params) {
    ///std::cout << sizeof... (params) << "\n";
    return (params + ... + 0);
}
int main()
{
    int x =  doit<int>(1, 2, 3, 4);
    std::cout << x<<"\n";
  /*  tuple<int, double, std::string> t;
    const int n = 10;
    std::vector<int> v(10);
    std::fill_n(v.begin(), n, 2);
    auto res=std::accumulate(v.begin(), v.end(),0
        , [](int x, int acc) { return  acc + x; }
        );
    std::cout << res<<"\n";*/
}
