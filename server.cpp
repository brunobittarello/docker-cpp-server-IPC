#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <cstring>
#include <string>

#include <cstdio>
#include <memory>
#include <stdexcept>
//#include <string>
#include <array>

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <ctime>
#include <iomanip>      // std::setw

using namespace std;

char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
"<style>"
"h1 { font-size:4cm; text-align: center;"
" text-shadow: 0 0 2mm white}</style></head>"
"<body><h1>";

char response2[] = "</h1></body></html>\r\n";
string result;
char serverfifo[] = "/serverfifo";
char clientfifo[] = "/clientfifo";

string time_log()
    {
        time_t rawtime;
        struct tm * timeContext;

        string temp;
        stringstream textStream;

        time(&rawtime);

        timeContext = localtime(&rawtime);

        textStream << put_time(timeContext, "%a %d %b %Y - %I:%M:%S " << rawtime;
        temp = textStream.str();

        return temp;
    }

string log_prefix()
{
	return time_log() + " - Server ";
}

int init_second_proc2()
{
        FILE *handle = popen("./myotherapp", "r");

        if (handle == NULL) {
                return 1;
        }

        char buf[4096];
        size_t readn;
        while ((readn = fread(buf, 1, sizeof(buf), handle)) > 0) {
			result.append(buf);
			cout << "\n" << "Buffer: " << buf;
            //fwrite(buf, 1, readn, stdout);
        }

        pclose(handle);

        return 0;
}

void init_second_proc()
{
	array<char, 4096> buffer;    
    unique_ptr<FILE, decltype(&pclose)> pipe(popen("./myotherapp", "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }

    result = "asd";
    cout << result << std::endl;
	/*
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result.append(buffer.data());
        cout << "\n" << "Result: " << result << std::endl;
        cout << "\n" << "Buffer: " << buffer.data() << std::endl;
    }
    */
    //printf("\Result:  %s\n", result);
    cout << "\n\n" << "Result Final: " << result;
}

int main()
{
	cout << log_prefix() << "started" << std::endl;

	char aString[4096];
	int one = 1, client_fd = 0;
	struct sockaddr_in svr_addr, cli_addr;
	socklen_t sin_len = sizeof(cli_addr);
	aString[0]='\0';
	strcpy(aString, "Docker tested - ");

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		err(1, "can't open socket");

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

	int port = 8080;
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
		close(sock);
		err(1, "Can't bind");
	}

	listen(sock, 5);
	int count = 0;
	char str1[80];
	int fd1;

	mkfifo(serverfifo, 0777);
	while (1) {
		client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
		cout << log_prefix() << "got connection" << std::endl;

		fd1 = open(serverfifo,O_WRONLY);
		cout << log_prefix() << "opened(write) server fifo " << fd1 << std::endl;
		if (fd1 == -1) return 3;

		char message[] = "Hello?";
		write(fd1, message, sizeof(message) + 1);
		cout << log_prefix() << "wrote" << std::endl;
		close(fd1);
		cout << log_prefix() << "closed server fifo " << fd1 << std::endl;

		fd1 = open(clientfifo,O_RDONLY);
		cout << log_prefix() << "opened(read) client fifo " << fd1 << std::endl;
		if (fd1 == -1) return 4;
		
		read(fd1, str1, 80);
		cout << log_prefix() << "server read: " << str1 << std::endl;
		close(fd1);
		cout << log_prefix() << "server closed client fifo" << std::endl;

		if (client_fd == -1) {
			perror("Can't accept");
			cout << log_prefix() << "Can't accep" << std::endl;
			continue;
		}

		write(client_fd, response, sizeof(response) - 1); /*-1:'\0'*/
		write(client_fd, aString, strlen(aString));
		char* s = &(std::to_string(count++))[0];
		write(client_fd, s, sizeof(s) - 1); /*-1:'\0'*/	
		char char_array[result.length() + 1];
		strcpy(char_array, result.c_str());
		write(client_fd, char_array, sizeof(char_array) - 1); /*-1:'\0'*/


		write(client_fd, str1, sizeof(str1) - 1); /*-1:'\0'*/


		/* Here some possible enhancements to read query, update it before resending to client */
		/*
		int bytes_read = read(client_fd, aString, 4096);
		aString[bytes_read] = '\0';
		if(bytes_read > 0)
		  write(client_fd, aString, strlen(aString));
		else
		  write(client_fd, response, sizeof(response) - 1);
		*/

		write(client_fd, response2, sizeof(response2) - 1); /*-1:'\0'*/   

		close(client_fd);
	}
}
