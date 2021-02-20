// tmp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
int main()
{
    const int n = 10;
    std::vector<int> v(10);
    std::fill_n(v.begin(), n, 2);
    auto res=std::accumulate(v.begin(), v.end(),0
        , [](int x, int acc) { return  acc + x; }
        );
    std::cout << res<<"\n";
}
