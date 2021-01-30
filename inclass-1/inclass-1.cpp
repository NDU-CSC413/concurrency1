/* create 2 threads.
* Each thread calls the same function is_prime shown below to 
* test if a number is prime or not
* is_prime takes two parametes: a long long integer and a bool
* which is passed by reference
*/
#include <iostream>
#include <thread>
#include <random>
/* a simple function to sample large random numbers */
std::random_device rd;
std::uniform_int_distribution<int> dist(4000000, 8000000);
inline int get_rand() {
    return dist(rd);
}

bool is_prime(int value) {
      /* this is NOT the most efficient way of
      * determining if a number is prime */
    if (value <= 1)return true;
    
    for (int i = 2; i < value; ++i) 
        if (value % i == 0) return false;
    
    return true;
}
/* samples random numbers until it finds the first prime.
* gives up and returns 0 after n tries
*/
int get_large_prime(int n) {
    int large;
    for (int i = 0; i < n; ++i) {
        large = get_rand();
        if (is_prime(large))return large;
    }
    return 0;
}
/* TODO: write a new version of get_large_prime
* to be launched from threads 
*/
/* write your code here. */
/* NOTE: give it a name different than get_large_prime 
* because it will passed to a thread constructor
* the compiler doesn't know which one you.
* refer to the notes for ways to keep two versions
* with the same name
*/

void get_large_prime(int n, int& result) {
    int large;
    result = 0;
    for (int i = 0; i < n; ++i) {
        large = get_rand();
        if (is_prime(large)) {
            result = large;
            return;
        }
    }
    return ;
}
int main()
{
    const int num_tries = 300;
    auto start = std::chrono::high_resolution_clock::now();
    int v1,v2;
    std::cout << "starting sequential\n";
    v1=get_large_prime(num_tries);
    v2 = get_large_prime(num_tries);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::ratio<1, 1000>>(end - start);
    std::cout << "values found " << v1 << " and " << v2 << std::endl;
    std::cout << "sequential time = " << duration.count() << std::endl;
    
    std::cout << " starting two threads \n";
    start = std::chrono::high_resolution_clock::now();
    /* start two threads here and call the
    * the function you wrote to get a large prime
    */
    using FType = void (*)(int, int&);
    FType f = get_large_prime;
    std::thread t1(f,num_tries, std::ref(v1));
    std::thread t2(f,num_tries, std::ref(v2));
    t1.join(); t2.join();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration<double, std::ratio<1, 1000>>(end - start);
  
    std::cout << "values found " << v1 << " and " << v2 << std::endl;
    std::cout << "two threads time = " << duration.count() << std::endl;
   
}
