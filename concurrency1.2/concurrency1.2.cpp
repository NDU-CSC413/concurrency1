// concurrency1.2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <thread>

void doit() {

}
void start() {
	std::thread t{ doit };
	t.detach();

}

int main()
{
	start();
	std::this_thread::sleep_for(std::chrono::minutes(5));
}

