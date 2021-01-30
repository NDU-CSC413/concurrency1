#include <thread>
#include <iostream>
#include <string>
/* Passing parameters to threads
*/

void threadf(std::string s) {
	std::cout << s << std::endl;
}

/* parameter by reference*/
void threadr(int& x) {
	x = 17;
}

struct foo {
	int& _x;
	foo(int& x) :_x(x) {}
	void operator()() {
		_x = 19;
	}
};

int main()
{	
	int x = 12;
	
	std::thread t1(threadf, "hello thread");
	/* internally the thread constructor passes values
	* as rvalue references*/

	// this does not compile
	//std::thread t2(threadr, x);

	std::thread t2(threadr, std::ref(x));
	t2.join();
	std::cout << x << std::endl;

	/*a different way would be to create 
	* a function object that contains a 
	* reference to x
	*/
	std::thread t3(foo{ x });
	t1.join();
	t3.join();
	std::cout << x << std::endl;

}
