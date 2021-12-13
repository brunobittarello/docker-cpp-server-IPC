#include <iostream>
#include <iomanip>      // std::setw
#include <string>   // std::string

#include <chrono>
#include <thread>

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


using namespace std;

char serverfifo[] = "/serverfifo";
char clientfifo[] = "/clientfifo";
char str1[80];

int main()
{
    std::cout << "XTestingOpenThisApp!\r\n";
    string mystring = "aqui";
    std::cout << "\r\n" << mystring << "\r\n" << std::endl;
	
	if (mkfifo(clientfifo, 0777)!= 0) {
		std::cout << "Oh dear, something went wrong with read()!" << errno << std::endl;
		return errno;
	}
	
	std::cout << "\r\nfoi\r\n" << std::endl;
	int fd1;
	
	while (1)
    {
		std::cout << "\r\n while \r\n" << std::endl;
		
		// Read only and read
        fd1 = open(serverfifo,O_RDONLY);
		if (fd1 == -1) {
			std::cout << "sleep" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
        read(fd1, str1, 80);
		close(fd1);
		std::cout << "\r\n read:" << str1 << "\r\n" << std::endl;
		
		// Now open in write mode and write
        // string taken from user.
        fd1 = open(clientfifo,O_WRONLY);
		std::cout << "\r\n open \r\n" << std::endl;
		if (fd1 == -1) return 2;
		std::cout << "\r\n fd1 \r\n" << std::endl;
		char lixo[] = "Sim";
        write(fd1, lixo, sizeof(lixo) + 1);
		std::cout << "\r\n write \r\n" << std::endl;
        close(fd1);        
		std::cout << "\r\n close \r\n" << std::endl;
		
    }
	
	/*
	for (int i = 0; i < 10; i++) {
		std::cout << "sleep" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	*/
}