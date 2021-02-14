#include "TCPServer.h"
#include "string.h"
#include <iostream>
#include <unistd.h>

std::mutex TCPServer::mutex_;

TCPServer::TCPServer()
{
    if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error("Socket creation failed");
    }
}

void TCPServer::send(int client, byte *data)
{
    int size = sizeof(data);

    if (::send(client, data, size, 0) < 0)
    {
        std::cout << "error sending data" << std::endl;
    }
}

void TCPServer::setFlag(int flag)
{
    int opts = fcntl(sockfd_, F_GETFL) | flag;

    if (fcntl(sockfd_, F_SETFL, opts) < 0)
    {
        error("set flag error");
    }
}

void TCPServer::setAddress(int address, int port)
{
    listen_addr_.sin_family = AF_INET;
    listen_addr_.sin_addr.s_addr = ntohl(address);
    listen_addr_.sin_port = htons(port);
};

void TCPServer::start(int port)
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

    setFlag(NON_BLOCKING);

    sockaddr address;
    socklen_t addr_length = sizeof(address);

    int clientfd;

    listening_ = true;

    printf("Waiting for Clients\n");

    while (listening_)
    {
        clientfd = accept(sockfd_, &address, &addr_length);

        if (clientfd < 0)
        {
            continue;
        }

        clients_.insert({clientfd, address});

        std::thread clientThread(&TCPServer::handleClient, this, clientfd);

        clientThread.detach();
    }
}

void TCPServer::handleClient(int clientfd)
{
    std::cout << "New Client!" << std::endl;

    int readBytes = 0;

    unsigned char buffer[BUFFER_SIZE];

    bool listening = true;

    while (listening)
    {
        readBytes = read(clientfd, buffer, BUFFER_SIZE);

        if (readBytes < 0)
        {
            std::cout << "Error reading bytes" << std::endl;
        }
        else if (readBytes == 0)
        {
            std::cout << "Client Disconnected" << std::endl;
            listening = false;

            TCPServer::mutex_.lock();

            clients_.erase(clientfd);

            TCPServer::mutex_.unlock();
        }
        else
        {
            if (delegate != nullptr)
            {
                TCPServer::mutex_.lock();

                delegate->onDataReceive(buffer, clientfd);
                
                TCPServer::mutex_.unlock();
            }

            memset(&buffer, 0, BUFFER_SIZE);
        }
    }
}