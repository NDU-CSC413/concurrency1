/*****************************************************************//**
 * @file   concurrency1.5.cpp
 * @brief  Parallel version of accumulate
 * 
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <numeric>
#include "../../include/utility.h"
using Long = unsigned long long;

/* assume the vector size is a power of 2 */
template <typename Iter,typename T,typename Func>
T parallel_acc(Iter first, Iter last, T init,Func f) {
	Long const length = std::distance(first, last);
	
	//Long num_threads = std::thread::hardware_concurrency();
	Long num_threads = 2;
	Long block_size = length / num_threads;
	std::vector<T> results(num_threads ) ;
	std::vector<std::thread> threads(num_threads );
	Iter block_start, block_end;
	for (Long i = 0; i < num_threads; ++i) {
		block_start = first + i * block_size;
		block_end = first+(i+1)*block_size;
		threads[i] = std::thread(
			[=](Iter s, Iter e, T& r) {
				r = std::accumulate(s, e, 0.0,f
				//,	[](double acc,T x) {return acc+=1.0 / (1.0 + x * x); }
				);
			
			},
			block_start,block_end,std::ref(results[i])
		);
	}
	
	for (auto& t : threads)
		t.join();
	
	return std::accumulate(results.begin(), results.end(), init);

}
int main()
{
	const int  n = 1<<23;
	std::vector<double> v(n);
	std::fill_n(v.begin(), n, 1.0);
	Duration d;
	double r;
	TIMEIT(d
		,r=parallel_acc(v.begin(), v.end(), 0.0
			, [](double acc, double val) {return acc += 1.0 / (1 + val * val); }
		);
	)
	std::cout << "result= " << r << std::endl;
	std::cout << "duration= " << d.count() << std::endl;
	/* reference for std::accumulate
	* https://tinyurl.com/yd9b4qz8
	*/
	TIMEIT(d
		, r=std::accumulate(v.begin(), v.end(), 0.0
			, [](double acc, double val) {return acc+=1.0 / (1 + val * val); }
		);
	)
	std::cout << r << "\n";
	std::cout << d.count() << "\n";
}

