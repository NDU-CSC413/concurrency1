

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <numeric>
using Long = unsigned long long;

/* assume the vector size is a power of 2 */
template <typename Iter,typename T>
T parallel_acc(Iter first, Iter last, T init) {
	Long const length = std::distance(first, last);
	
	Long num_threads = std::thread::hardware_concurrency();
	Long block_size = length / num_threads;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads );
	Iter block_start, block_end;
	for (Long i = 0; i < num_threads; ++i) {
		block_start = first + i * block_size;
		block_end = first+(i+1)*block_size;
		threads[i] = std::thread(
			[=](Iter s, Iter e, T& r) {
				r = std::accumulate(s, e, 0.0);
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
	const int  n = 10000000;
	std::vector<int> v(n);
	std::fill_n(v.begin(), n, 1);
	int r=parallel_acc(v.begin(), v.end(), 0);
	std::cout << "number of threads =" << std::thread::hardware_concurrency() << std::endl;
	std::cout << "result= " << r << std::endl;
}

