#include "socket.cpp"
#include <map>

#define INT_SIZE 32

class UDPServer : public UDPSocket
{
private:
    sockaddr_in last_client_;
    std::map<int, sockaddr_in> clients;

    char *generateKey(sockaddr_in address)
    {
        char *ip = inet_ntoa(address.sin_addr);

        int length = INT_SIZE + 2;
        char port[length];

        for (int i = 0; i < INT_SIZE; i++)
        {
            port[i] = (char)(((address.sin_port >> i) & 1) + 48);
        }

        port[length-1] = '\0';

        strcat(ip, port);

        return ip;
    }

public:
    void listen()
    {
        if (bind(sockfd, (sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
        {
            error("Socket binding failed");
        }

        UDPSocket::listen();
    }

    void send(char *message)
    {
        strcat(message, "\n");
        sendMessage(message, last_client_);
    }

    void onMessageReceive(char *message, sockaddr_in address)
    {
        std::cout << message;

        char *key = generateKey(address);

        std::cout << key << std::endl;

        last_client_ = address;
    }
};