#include <thread>
#include <iostream>
#include <string>
#include <functional>

// replace EXAMPLE1 by EXAMPLE2 to run the second example
#define EXAMPLE1

#ifdef EXAMPLE1
void threadf(std::string s) {
	std::cout << s << std::endl;
}
#endif 
#ifdef EXAMPLE2
void threadf(int& x) {
	x = 17;
}
#endif 


int main()
{	
	int x = 12;
	/* internally the thread constructor passes values
	* as rvalue references*/
#ifdef EXAMPLE1
	std::thread t(threadf, "hello thread");
#endif
#ifdef EXAMPLE2
	std::thread t(threadf, std::ref(x));
#endif 
	t.join();
#ifdef EXAMPLE2
	std::cout << x << std::endl;
#endif 
}
