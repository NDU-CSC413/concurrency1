

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <numeric>
#include <chrono>
#include <cmath>
#include <execution>

/* parallel  dot product of two vector
* where we assume the size of the
* vectors is a power of two
*/

using Long = unsigned long long;
/* take the cosine of the product instead
* of the product to increase the cost
* of computation
*/
struct fo {
	double operator ()(double x, double y) {
		//return std::cos(x * y);
		return x * y;
	}
};
/* performs binary OP between two ranges 
* and accumulate the result
*/
template <typename Iterator, typename T>
inline void	acc_block(Iterator first1, Iterator last1,Iterator first2,Iterator last2,
		T& result) {
		result=std::inner_product(first1, last1, first2, 0.0);
	}

template <typename Iterator>
double inner_par(Iterator first1, Iterator last1,Iterator first2,Iterator last2) {
	Long length = std::distance(first1, last1);
	if (std::distance(first2, last2) != length) {
		std::cout << "different ranges\n";
		return 0;
	}
	
	Long num_threads = std::thread::hardware_concurrency();
	Long block_size = length / num_threads;

	std::vector<double> results(num_threads);
	std::vector<std::thread> threads(num_threads);
	
	Iterator block_start1, block_end1, block_start2, block_end2;
	for (Long i = 0; i < num_threads; ++i) {
		block_start1 = first1+i*block_size;
		block_end1 = first1 + (i + 1) * block_size;
		block_start2 = first2 + i * block_size;
		block_end2 = first2 + (i + 1) * block_size;

		threads[i] = std::thread(acc_block<Iterator,double>,
			block_start1, block_end1,block_start2,block_end2, 
			std::ref(results[i]));

	}
	for (auto& t : threads)
		t.join();
	return std::accumulate(results.begin(), results.end(), 0.0);

}
int main()
{
	int const n = 50000000;
	std::vector<double> u(n);
	std::vector<double> v(n);
	std::fill_n(v.begin(), n, 6);
	std::fill_n(u.begin(), n, 7);
	auto start=std::chrono::high_resolution_clock::now();
	double r = inner_par(v.begin(), v.end(), u.begin(), u.end());
	//double r = parallel_acc(v.begin(), v.end(), u.begin(), u.end(), 0, std::multiplies<double>{});
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double,std::milli> diff = end - start;
	std::cout << "number of threads =" << std::thread::hardware_concurrency() << std::endl;
	std::cout << "result= " 	<<r << std::endl;
	std::cout << "time = " << diff.count() << std::endl;
	start = std::chrono::high_resolution_clock::now();
	//r = dot(v.begin(), v.end(), u.begin(), u.end(), 0, std::multiplies<double>{});
	//r = dot(v.begin(), v.end(), u.begin(), u.end(), 0.0,fo{});
	r = std::inner_product( v.begin(), v.end(), u.begin(), 0.0);
	end = std::chrono::high_resolution_clock::now();
	diff = end - start;
	std::cout << "result = " << r << "  time = " << diff.count() << std::endl;
}

