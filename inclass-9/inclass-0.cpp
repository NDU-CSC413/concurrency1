
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
void read_input() {
    std::string s;
    std::cout << "enter a string. Type 'quit' to quit\n";
    while (s != "quit") {
        std::cin >> s;
        std::cout << "user input= " << s << "\n";

    }
}
void read_file(std::string name) {
    std::ifstream file;
    std::string content;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    file.open(name);
    std::getline(file, content);
    std::cout << "content of file =" << content << "\n";
    file.close();
}
int main()
{
    std::thread t(read_file,"input.txt");
    std::thread t2(read_input);
    t.join();
    t2.join();
    std::cout << "main thread done\n";
}
