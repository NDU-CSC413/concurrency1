

#include <iostream>
#include <vector>
#include <thread>
#include <fstream>

/* illustration of detach
* when the main thread exits all other
* threads are destroyed and therefore
* might not have enough time to complete
* their task
*/

#define WAIT

void write_to_file() {
	std::ofstream output;
	output.open("output.txt");
	output << "first line\n";
	output << "second line\n";
	output.close();
}

int main()
{
	std::thread t{ write_to_file };
	t.detach();
#ifdef WAIT
	std::this_thread::sleep_for(std::chrono::minutes(1));
#endif
}

