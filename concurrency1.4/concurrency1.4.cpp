//transfer of ownership of threads

#include <iostream>
#include <thread>
#include <vector>
#define EXAMPLE4
void threadf() {
    std::cout << "hello thread\n";
}
int main()
{
   
#ifdef EXAMPLE1
    //compile error. threads are not copyable
    std::thread t(threadf);
    std::thread g = t;
    g.join();
#endif

#ifdef EXAMPLE2
    //ok transferring ownership from t to g
    std::thread t(threadf);
    std::thread g = std::move(t);
    g.join();
#endif 


#ifdef EXAMPLE3
    //compile error. "adding" threads to container entails copying
    std::vector<std::thread> mythreads;
    std::thread t(threadf);
    mythreads.push_back(t);
    std::thread g(threadf);
    mythreads.push_back(g);

    for (auto& t : mythreads)
        t.join();
#endif 


#ifdef EXAMPLE4
    //Ok. threads are transferred to the container
    std::vector<std::thread> mythreads;
    std::thread t(threadf);
    mythreads.push_back(std::move(t));
    std::thread g(threadf);
    mythreads.push_back(std::move(g));

    for (auto& t : mythreads)
        t.join();
#endif 

}

