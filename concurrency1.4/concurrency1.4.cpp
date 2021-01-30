#include <iostream>
#include <thread>

/*transfer of ownership for threads
* There are three examples: EXAMPLE1,EXAMPLE2, and EXAMPLE3
* define the one you want to run
* EXAMPLE1 gives a compile error because threads are not
* copyable.
* EXAMPLE2 is a fix using template specialization
* EXAMPLE3 is a better fix using rvalue references.
*/
#define EXAMPLE3
void threadf() {

}
#if defined( EXAMPLE1)|| defined(EXAMPLE2)
template<typename T>
struct Container {
	T _t;
	Container() {
		std::cout << "general\n";
	}
	void addItem(const T& t) {
		_t = t;
	}
	T& getItem() {
		return _t;
	}
};
#endif 
#if defined(EXAMPLE2)
template<>
struct Container<std::thread> {
	std::thread _t;
	Container() {
		std::cout << "specialized\n";
	}
	void addItem(std::thread& t) {
		_t = std::move(t);
		
	}
	std::thread & getItem() {
		return _t;
	}
	
};
#endif
#if defined(EXAMPLE3)
template<typename T>
struct Container {
	T _t;
	void addItem(T& t) {
		_t = t;
	}
	void addItem(T&& t) {
		_t = std::move(t);
	}
	T& getItem() {
		return _t;
	}
};
#endif

struct Tclass{
};

int main() {

	std::thread t(threadf);
	Tclass obj;
	Container<std::thread> tc;
	Container<Tclass> oc;
#ifdef EXAMPLE3
	tc.addItem(std::move(t));
#else
	tc.addItem(t);
#endif 
	oc.addItem(obj);
	
	tc.getItem().join();
	std::cout << "main thread is done\n";
}