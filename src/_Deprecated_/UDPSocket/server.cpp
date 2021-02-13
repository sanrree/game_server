#include <map>
#include "socket.cpp"

#define INT_SIZE 32

class UDPServer : public UDPSocket
{
public:
    void listen()
    {
        if (bind(sockfd, (sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
        {
            error("Socket binding failed");
        }

        UDPSocket::listen();
    }
};