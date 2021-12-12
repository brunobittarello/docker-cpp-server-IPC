#include <iostream>
#include <iomanip>      // std::setw
#include <string>   // std::string

#include <chrono>
#include <thread>


using namespace std;

int main()
{
    std::cout << "XTestingOpenThisApp!\r\n";
    string mystring = "aqui";
    std::cout << "\r\n" << mystring << "\r\n" << std::endl;
	/*
	while(1) {
		std::cout << "sleep" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	*/
}