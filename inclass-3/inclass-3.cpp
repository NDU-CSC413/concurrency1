/* Given a vector v of n integers:
* Write a function to test if v  
* contains a certain number at least once.
* Use as many threads as your hardware permits.
*/

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <execution>
#include "../inclass-2/utitlities.h"

#define PART2
/* note that the range is
* a semi closed interval [first,last)
*/

template<typename Iter,typename T,typename IterBool> 
void contains(Iter first, Iter last, T value,IterBool result);

/* see the lecture notes for the reason we use
* an iterator for the bool instead of passing a bool
* variable by reference
*/
template<typename Iter,typename T,typename IterBool>
void contains(Iter first, Iter last, T value,IterBool result) {
	Iter itr = first;
	*result = false;
	while (itr != last) {
		if (*itr == value) {
			*result = true;
			return;
		}
		++itr;
	}
	return ;
}

int main()
{
	/* choose size power of two */
	const int n = 2 << 25;
	std::random_device rd;
	std::uniform_int_distribution<int> dist(1, 10000);
	/* choosing a value that is NOT in the vector*/
	int value = 12000;


	std::vector<int> v(n);
	std::generate(v.begin(), v.end(), [&]() {return dist(rd); });
	using Iter = std::vector<int>::iterator;
	/* using the sequential STL std::any_of() */
	auto start = std::chrono::high_resolution_clock::now();
	bool res = std::any_of(v.begin(), v.end(), [=](int x) {return x == value; });
	std::cout << "sequential result = " << std::boolalpha << res << std::endl;
	auto end = std::chrono::high_resolution_clock::now();
	print_duration(start, end, "sequential ");

	/* Using our implementation with the number of
	* threads equal to the hardware threads
	*/
	int num_threads = std::thread::hardware_concurrency();
	int block_size = n / num_threads;
	/* store the result of each block in vector results */
	std::vector<bool> results(num_threads);
	std::vector<std::thread> mythreads;
	Iter begin = v.begin();
	auto itr = results.begin();
	start = std::chrono::high_resolution_clock::now();
	/* declaring i as long long to avoid warning
	* when we use it for iterator arithmetics
	* since iterators on x64 are 8 bytes
	*/
	/* divide the input into blocks equal to the
	* number of threads
	*/
	for (long long i = 0; i < num_threads; ++i) {
		/* we can do such arithmetic because
		* vector iterators are random iterators
		*/
		Iter first = begin + i * block_size;
		Iter last = begin + (i + 1) * block_size;
		mythreads.push_back(
			std::thread(
				contains<Iter, int, decltype(itr)>, first, last, value, itr)
		);
		++itr;
	}
	for (auto& t : mythreads)
		t.join();
	end = std::chrono::high_resolution_clock::now();
	print_duration(start, end, "multiple threads");
	constexpr auto identity = [](auto r) {return r; };
	/* combine the results of blocks */
	std::cout<<"multiple thread result "<<
		std::any_of(results.begin(), results.end(),
			identity)<< "\n";
	
}

