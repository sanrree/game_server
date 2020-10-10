#include <iostream>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

class UDPSocket
{
private:
protected:
    UDPSocket()
    {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            error("Socket creation failed");
        }
    }

    int sockfd;

    int recieved_msgs;

    char buffer[BUFFER_SIZE], message[BUFFER_SIZE];

    sockaddr_in recv_addr, send_addr;

    void startInputListener()
    {
        socklen_t send_addr_size = sizeof(send_addr);

        while (1)
        {
            std::cin.getline(message, sizeof(message));
            strcat(message, "\n");
            sendto(sockfd, message, strlen(message), MSG_DONTROUTE, (sockaddr *)&send_addr, send_addr_size);
            memset(&message, 0, BUFFER_SIZE);
        }
    }

    void startInputListenerAsync()
    {
        std::thread t(&UDPSocket::startListenerLoop, this);
        t.detach();
    }

    void startListenerLoop()
    {
        socklen_t send_addr_size = sizeof(send_addr);

        while (1)
        {
            recieved_msgs = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&send_addr, &send_addr_size);

            if (recieved_msgs > 0)
            {
                std::cout << inet_ntoa(send_addr.sin_addr) << std::endl;
                std::cout << buffer;
                memset(&buffer, 0, BUFFER_SIZE);
            }
        }
    }

public:
    virtual void start(const char *address, int port) = 0;
};