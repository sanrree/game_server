#include <iostream>
#include <string.h>
#include <thread>
#include "socketHeader.h"

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

    byte buffer[BUFFER_SIZE];

    sockaddr_in listen_addr, recv_addr;

public:
    SocketReceiverDelegate *delegate = nullptr;

    virtual void listen()
    {
        socklen_t recv_addr_size = sizeof(recv_addr);

        while (1)
        {
            recieved_msgs = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (sockaddr *)&recv_addr, &recv_addr_size);

            if (recieved_msgs > 0)
            {
                if (delegate != nullptr)
                {
                    (*delegate).onMessageReceive(buffer, recv_addr);
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

    void sendMessage(char *message, int size, sockaddr_in address)
    {
        socklen_t addr_size = sizeof(address);
        sendto(sockfd, message, size, MSG_DONTROUTE, (sockaddr *)&address, addr_size);
    };

    static bool compareAdresses(sockaddr_in a, sockaddr_in b)
    {
        return (a.sin_addr.s_addr == b.sin_addr.s_addr) &&
               (a.sin_port == b.sin_port);
    }
};