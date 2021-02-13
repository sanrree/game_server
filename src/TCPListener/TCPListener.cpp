#include "TCPListener.h"
#include "string.h"
#include <iostream>

TCPListener::TCPListener()
{
    if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error("Socket creation failed");
    }
}

void TCPListener::send(const char *data, sockaddr address)
{
    socklen_t addr_size = sizeof(address);

    int size = sizeof(data);

    sendto(sockfd_, data, size, MSG_DONTROUTE, &address, addr_size);
}

void TCPListener::setFlag(int flag)
{
    int opts = fcntl(sockfd_, F_GETFL) | flag;

    if (fcntl(sockfd_, F_SETFL, opts) < 0)
    {
        error("set flag error");
    }
}

void TCPListener::setAddress(int address, int port)
{
    listen_addr_.sin_family = AF_INET;
    listen_addr_.sin_addr.s_addr = address;
    listen_addr_.sin_port = htons(port);
};

void TCPListener::start(int port)
{
    setAddress(INADDR_ANY, port);

    if (bind(sockfd_, (sockaddr *)&listen_addr_, sizeof(listen_addr_)) < 0)
    {
        error("bind failed");
    }

    if (listen(sockfd_, 3) < 0)
    {
        error("listen error");
    }

    //setFlag(NON_BLOCKING);

    sockaddr address;
    socklen_t addr_length = sizeof(address);

    int clientfd;

    listening_ = true;

    while (listening_)
    {
        clientfd = accept(sockfd_, NULL, NULL);

        if (clientfd < 0)
        {
            printf("No Client");
            continue;
        }

        printf("New Client");
        // std::thread clientThread(&TCPListener::handleClient, this, clientfd);

        // clientThread.detach();
    }
}

void TCPListener::handleClient(int clientfd)
{
    printf("New client");
}