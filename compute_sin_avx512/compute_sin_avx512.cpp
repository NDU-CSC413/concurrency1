/*****************************************************************//**
 * @file   compute_sin_avx512.cpp
 * @brief  Computation of the sine function up to TERMS
 * of the Taylor expansion.
 * with different versions: sequential,multithreaded,
 * avx512, multithreaded avx512
 * The functions that actuall perform the computation
 * are: seq_sin and vec_sin. To factor out the overhead
 * of managing threads internally each of these two 
 * functions repeat, needlessly, the computation a
 * number of times equal to the defined LOAD
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/
#include <iostream>
#include <thread>
#include <utility>
#include <chrono>
#include <vector>
#include <cmath>
#include <random>
#include <immintrin.h>
#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef TERMS
#define TERMS 5
#pragma message (" TERMS is set to 5. Low accuracy ")
#endif // !TERMS
#define LOAD 1


#include "immintrin.h"
using Long = unsigned long long;
using Duration = std::chrono::duration<double, std::milli>;

#define TIMEIT(dur,...)\
   {\
    auto start = std::chrono::high_resolution_clock::now();\
    __VA_ARGS__\
    auto end = std::chrono::high_resolution_clock::now();\
     dur = std::chrono::duration<double, std::milli>(end - start);\
}
const int n = 1 << 24;


float a[n], b[n];
float c[n];

void seq_sin(int n,int terms, float* a, float* b) {
	for (int k = 0; k < LOAD; ++k) {
		for (int i = 0; i < n; ++i) {
			float x = a[i];
			float value = x;
			float num = x * x * x;
			float denom = 6;
			int sign = -1;
			for (int j = 1; j <= terms; ++j) {
				value += sign * num / denom;
				num *= x * x;
				denom *= (2 * j + 2) * (2 * j + 3);
				sign *= -1;
			}
			b[i] = value;
		}
	}
}

void vector_sin(int n,int terms, float* a, float* b)
{
	for (int k = 0; k < LOAD; ++k) {
		for (int i = 0; i < n; i += 16)
		{
			__m512 x = _mm512_load_ps(&a[i]);
			__m512 value = x;
			__m512 numer = _mm512_mul_ps(x, _mm512_mul_ps(x, x));//x^3
			__m512 denom = _mm512_set1_ps(6);// 3 factorial
			int sign = -1;
			for (int j = 1; j <= terms; j++)
			{
				// value += sign * numer / denom
				__m512 tmp = _mm512_div_ps(_mm512_mul_ps(_mm512_set1_ps(sign), numer), denom);
				value = _mm512_add_ps(value, tmp);
				numer = _mm512_mul_ps(numer, _mm512_mul_ps(x, x));
				denom = _mm512_mul_ps(denom, _mm512_set1_ps((2 * j + 2) * (2 * j + 3)));
				sign *= -1;
			}
			_mm512_store_ps(&b[i], value);
		}
	}
}
template<typename F>
void multit_sin(int terms, float* x, float* y,F f) {
	int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> mythreads;
	int block_size = n / num_threads;
		for (int i = 0; i < num_threads; ++i) {

			mythreads.push_back(
				//std::thread(seq_sin, block_size, TERMS, x, y)
				std::thread(f, block_size, TERMS, x, y)
			);
			x += block_size;
			y += block_size;
		}
	
	for (int i = 0; i < num_threads; ++i)
		mythreads[i].join();

}
int main() {
	/*std::cout << "Executing \n";
	std::cout << QUOTE(FUNC(OPT)) << "\n";*/
	std::cout << "Array size = " << n << "\n";
	std::random_device rd;
	std::uniform_real_distribution<float> dist(0, 4);
	for (int i = 0; i < n; ++i)a[i] = dist(rd);
	Duration d_seq{}, d_vec{}, d_m{}, d_mvec{}, d_stl{}, d{};
	int num_runs = 1;
	//d_sec.zero(); d_vec.zero(); d_m.zero(); d_mvec.zero(); d_stl.zero();
	std::cout << "duration(ms) of the computation of sine ("<<LOAD<<" times ) for array of size="
		<< n << "\n";

	for (int i = 0; i < num_runs; ++i) {
		/* dry run*/
		TIMEIT(d, seq_sin(n, TERMS, a, b);
		)

		/* multithreaded sequential */
		TIMEIT(d, multit_sin(TERMS, a, b, seq_sin);
		)
			d_m += d;
		/* multithreaded vector */
		TIMEIT(d, multit_sin(TERMS, a, b, vector_sin);
		)
			d_mvec += d;

		/* sequenial */
		TIMEIT(d, seq_sin(n, TERMS, a, b);
		)
			d_seq += d;
		
		/* vector */
		TIMEIT(d, vector_sin(n, TERMS, a, b);
		)
			d_vec += d;
		
		/* STL */
		TIMEIT(d, for (int i = 0; i < n; ++i)c[i] = sin(a[i]);
		)
			d_stl += d;
		//std::cout << "STL=\t\t\t   " << d.count() << "\n";
	}
	std::cout << "sequential=\t\t   " << d_seq.count()/num_runs <<"   1\n";
	std::cout << "multithreaded sequential=  " << d_m.count() / num_runs << "   "
		<<d_seq.count()/d_m.count()<< "\n";
	std::cout << "avx512=\t\t\t   " << d_vec.count()/num_runs<<"   "
		<<d_seq.count()/d_vec.count()<<"\n";
	std::cout << "multithreaded avx512=\t   " << d_mvec.count()/num_runs <<"   "
		<< d_seq.count()/d_mvec.count() << "\n";



	/*std::cout << " compare few values with STL result \n";
	for (int i = 100; i < 105; ++i)std::cout << b[i] << ","; std::cout << "\n";
	for (int i = 100; i < 105; ++i)std::cout << c[i] << ","; std::cout << "\n";
	std::cout << "\n";*/
}
// you can run the code on intel compiler here https://godbolt.org/z/bs1sYr