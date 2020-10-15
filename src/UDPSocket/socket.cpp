#include <iostream>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define NON_BLOCKING O_NONBLOCK
#define BUFFER_SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

class UDPSocket
{
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

    char buffer[BUFFER_SIZE];

    sockaddr_in listen_addr, recv_addr;

public:
    virtual void setListenerAddress(const char *address, int port)
    {
        listen_addr.sin_family = AF_INET;
        listen_addr.sin_addr.s_addr = inet_addr(address);
        listen_addr.sin_port = htons(port);
    };

    virtual void listen()
    {
        socklen_t recv_addr_size = sizeof(recv_addr);

        while (1)
        {
            recieved_msgs = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&recv_addr, &recv_addr_size);

            if (recieved_msgs > 0)
            {
                char tmp[sizeof(buffer)];
                strcpy(tmp, buffer);

                onMessageReceive(tmp, recv_addr);

                memset(&buffer, 0, BUFFER_SIZE);
                memset((char *)&recv_addr, 0, recv_addr_size);
            }
        }
    }

    virtual void sendMessage(char *message, sockaddr_in address)
    {
        socklen_t addr_size = sizeof(address);
        sendto(sockfd, message, strlen(message), MSG_DONTROUTE, (sockaddr *)&address, addr_size);
    };

    virtual void setFlag(int flag)
    {
        int opts = fcntl(sockfd, F_GETFL) | flag;

        if (fcntl(sockfd, F_SETFL, opts) < 0)
        {
            error("set flag error");
        }
    }

    virtual void onMessageReceive(char *message, sockaddr_in address) = 0;
};