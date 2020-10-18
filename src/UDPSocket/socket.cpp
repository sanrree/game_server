#include <iostream>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "./socketReceiverDelegate.cpp"

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
    SocketReceiverDelegate *delegate = nullptr;

    virtual void listen()
    {
        socklen_t recv_addr_size = sizeof(recv_addr);

        while (1)
        {
            recieved_msgs = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (sockaddr *)&recv_addr, &recv_addr_size);

            if (recieved_msgs > 0)
            {
                char tmp[sizeof(buffer)];
                strcpy(tmp, buffer);

                if (delegate != nullptr)
                {
                    (*delegate).onMessageReceive(tmp, recv_addr);
                }

                memset(&buffer, 0, BUFFER_SIZE);
                memset((char *)&recv_addr, 0, recv_addr_size);
            }
        }
    }

    void setListenerAddress(const char *address, int port)
    {
        setListenerAddress(inet_addr(address), port);
    };

    void setListenerAddress(int address, int port)
    {
        listen_addr.sin_family = AF_INET;
        listen_addr.sin_addr.s_addr = address;
        listen_addr.sin_port = htons(port);
    };

    void setFlag(int flag)
    {
        int opts = fcntl(sockfd, F_GETFL) | flag;

        if (fcntl(sockfd, F_SETFL, opts) < 0)
        {
            error("set flag error");
        }
    }

    void sendMessage(char *message, sockaddr_in address)
    {
        socklen_t addr_size = sizeof(address);
        sendto(sockfd, message, strlen(message), MSG_DONTROUTE, (sockaddr *)&address, addr_size);
    };

    static bool compareAdresses(sockaddr_in a, sockaddr_in b)
    {
        return (a.sin_addr.s_addr == b.sin_addr.s_addr) &&
               (a.sin_port == b.sin_port);
    }
};