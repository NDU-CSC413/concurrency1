

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <numeric>
template <typename Iterator, typename T>
struct acc_block {
	void operator()(Iterator first1, Iterator last1,Iterator first2,Iterator last2, T& result) {
		unsigned long long const length = std::distance(first2, last2);
		if (std::distance(first2, last2) != length) {
			std::cout << "different ranges\n";
		}
		std::vector<T> w(length);
		std::transform(first1, last1, first2, w.begin(), std::multiplies<T>{});
		result = std::accumulate(w.begin(), w.end(), result);
	}

};

template <typename Iterator, typename T>
T parallel_acc(Iterator first1, Iterator last1,Iterator first2,Iterator last2, T init) {
	unsigned long long const length = std::distance(first1, last1);
	if (std::distance(first2, last2) != length) {
		std::cout << "different ranges\n";
		return 0;
	}
	std::vector<T> w(length);
	unsigned long const num_threads = std::thread::hardware_concurrency();
	unsigned long const block_size = length / num_threads;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	Iterator block_start1 = first1;
	Iterator block_start2 = first2;
	for (unsigned long i = 0; i < num_threads - 1; ++i) {
		Iterator block_end1 = block_start1;
		Iterator block_end2 = block_start2;
		std::advance(block_end1, block_size);
		std::advance(block_end2, block_size);
		threads[i] = std::thread(acc_block<Iterator, T>{},
			block_start1, block_end1,block_start2,block_end2, std::ref(results[i]));
		block_start1 = block_end1;
		block_start2 = block_end2;

	}
	acc_block<Iterator, T>()(block_start1, last1,block_start2,last2, results[num_threads - 1]);
	for (auto& t : threads)
		t.join();
	return std::accumulate(results.begin(), results.end(), init);

}
int main()
{
	int const n = 10000000;
	std::vector<int> u(n);
	std::vector<int> v(n);
	std::fill_n(v.begin(), n, 3);
	std::fill_n(u.begin(), n, 2);
	int r = parallel_acc(v.begin(), v.end(),u.begin(),u.end(), 0);
	std::cout << "number of threads =" << std::thread::hardware_concurrency() << std::endl;
	std::cout << "result= " << r << std::endl;
}

