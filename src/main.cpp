#include <iostream>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT_NO 8081
#define BUFFER_SIZE 1024

int sockfd, recieved_msgs;

char buffer[BUFFER_SIZE], message[BUFFER_SIZE];

sockaddr_in serv_addr, client_addr;

socklen_t client_size = sizeof(client_addr);

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void startInputListener()
{
	while (1)
	{
		std::cin.getline(message, sizeof(message));
		strcat(message, "\n");
		sendto(sockfd, message, strlen(message), MSG_DONTROUTE, (sockaddr *)&client_addr, client_size);
		memset(&message, 0, BUFFER_SIZE);
	}
}

void startListenerLoop()
{
	while (1)
	{
		recieved_msgs = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_size);

		if (recieved_msgs > 0)
		{
			std::cout << buffer;
			memset(&buffer, 0, BUFFER_SIZE);
		}
	}
}

int main()
{
	memset(&message, 0, BUFFER_SIZE);
	memset(&buffer, 0, BUFFER_SIZE);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		error("Socket creation failed");
		return 0;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT_NO);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("Socket binding failed");

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
	{
		error("fcntl call error");
	}

	std::thread t(startListenerLoop);
	t.detach();

	startInputListener();

	return 0;
}