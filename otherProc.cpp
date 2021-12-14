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

#include <ctime>

using namespace std;

char serverfifo[] = "/serverfifo";
char clientfifo[] = "/clientfifo";
char str1[80];

string time_log() 
{
	time_t rawtime;
	struct tm * timeContext;

	string temp;
	stringstream textStream;

	time(&rawtime);

	timeContext = localtime(&rawtime);

	textStream << put_time(timeContext, "%a %d %b %Y - %I:%M:%S ") << rawtime;
	temp = textStream.str();

	return temp;
}

string log_prefix()
{
	return time_log() + " - Program ";
}

int main()
{
    std::cout << log_prefix() << "started\r\n";	
	
	if (mkfifo(clientfifo, 0777)!= 0) {
		std::cout << log_prefix() << "Something went wrong with read()!" << errno << std::endl;
		return errno;
	}
	
	int fd1;
	while (1)
    {
		std::cout << log_prefix() << "waiting server" << std::endl;
		
		// Read only and read
        fd1 = open(serverfifo,O_RDONLY);
		cout << log_prefix() << "opened(read) server fifo " << fd1 << std::endl;
		if (fd1 == -1) {
			std::cout << log_prefix() << "is sleeping" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
        read(fd1, str1, 80);
		std::cout << log_prefix() << "read:" << str1 << std::endl;
		close(fd1);
		cout << log_prefix() << "closed server fifo " << fd1 << std::endl;
		
		
		// Now open in write mode and write
        // string taken from user.
        fd1 = open(clientfifo,O_WRONLY);
		cout << log_prefix() << "opened(write) client fifo " << fd1 << std::endl;
		if (fd1 == -1) return 2;
		char message[] = "Sim";
        write(fd1, message, sizeof(message) + 1);
		std::cout << log_prefix() << "wrote " << message << std::endl;
        close(fd1);        
		cout << log_prefix() << "closed client fifo " << fd1 << std::endl;
    }
}