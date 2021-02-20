/*****************************************************************//**
 * @file   concurrency1-6.cpp
 * @brief  Compute PI
 * PI=\int_0^1 4*DX/(1+x^2)
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/
#include <iostream>
#include <thread>
#include <iomanip>
#include <mutex>
#include <vector>
#include "../include/utility.h"

std::mutex m;
#define VER2
void helper(double stepsize,int from,int to,double& res) {
	double sum = 0, midpoint;
	for (int i = from; i < to; ++i) {
		midpoint = (i + 0.5) *stepsize;
		sum += 1.0 / (1 + midpoint * midpoint);
	}
	std::lock_guard<std::mutex> g(m);
	res += 4.0 *stepsize * sum;
}
template<int hard_t=2>
void par_pi(int pow, double& pi) {
	const int num_steps = 1 << pow;
	const int block_size = num_steps / hard_t;
	std::vector<std::thread> mythreads;
	double results[hard_t];
	double dx = 1.0 / (double)num_steps;
	pi = 0;
	for (int t = 0; t < hard_t;++t) {
		mythreads.push_back(
			std::thread(
				helper, dx, t * block_size, (t + 1) * block_size
#ifdef VER1
				,std::ref(results[t])
#else
				, std::ref(pi)
#endif 
			)
		);
	}
	
	for (auto& t : mythreads)t.join();
#ifdef VER1
	pi = 0;
	for (int i = 0; i < hard_t; ++i)
		pi += results[i];
#endif // VER1

}

	
void seq_pi(int pow,double& pi) {
	const int num_steps = 1 << pow;
	helper(1.0 / num_steps, 0, num_steps, pi);
}

int main(){
	
	double pi = 0;
	Duration d;
	TIMEIT(d
		, par_pi<8>(28, pi);
	)
	std::cout <<std::setprecision(20) << pi << "\n";
	std::cout << d.count() << "\n";

}
