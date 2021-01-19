

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <numeric>
template <typename Iterator,typename T>
struct acc_block {
	void operator()(Iterator first, Iterator last, T& result) {

		result = std::accumulate(first, last, result);
	}

};

template <typename Iterator,typename T>
T parallel_acc(Iterator first, Iterator last, T init) {
	unsigned long long const length = std::distance(first, last);
	/*unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads=std::min(*/
	unsigned long const num_threads = std::thread::hardware_concurrency();
	unsigned long const block_size = length / num_threads;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	Iterator block_start = first;
	for (unsigned long i = 0; i < num_threads - 1; ++i) {
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(acc_block<Iterator, T>{},
			block_start, block_end, std::ref(results[i]));
		block_start = block_end;

	}
	acc_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
	for (auto& t : threads)
		t.join();
	return std::accumulate(results.begin(), results.end(), init);

}
int main()
{
	int const n = 10000000;
	std::vector<int> v(n);
	std::fill_n(v.begin(), n, 1);
	int r=parallel_acc(v.begin(), v.end(), 0);
	std::cout << "number of threads =" << std::thread::hardware_concurrency() << std::endl;
	std::cout << "result= " << r << std::endl;
}

