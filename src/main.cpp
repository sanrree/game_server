#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <thread>
#include <unistd.h>

#define PORT_NO 8081
#define BUFFER_SIZE 1024

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void listen_input(char *str)
{
	while (1)
	{
		std::cin >> str;
	}
}

int main()
{
	int sockfd, newsockfd;

	struct sockaddr_in serv_addr, client_addr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		error("Socket creation failed");
		return 0;
	}

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = atol("127.0.0.1");
	serv_addr.sin_port = htons(PORT_NO);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Socket binding failed");
	}

	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	listen(sockfd, 1);

	int n;

	int buffersize = 1024;
	char buffer[BUFFER_SIZE];

	socklen_t len = (int)sizeof(client_addr);

	char message[BUFFER_SIZE];

	std::thread t(listen_input, message);
	t.detach();

	while (1)
	{

		n = recvfrom(sockfd, (char *)buffer, buffersize, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);

		if (n > 0)
		{
			std::cout << "asdasda";
			std::cout << buffer;
			memset(&buffer, 0, buffersize);
		}

		if (message[0] != '\0')
		{
			std::cout << message << std::endl;
			message[0] = '\0';

			sendto(sockfd, message, strlen(message),
				   MSG_DONTROUTE, (const struct sockaddr *)&client_addr, len);
		}
	}

	return 0;
}