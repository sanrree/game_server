#include "socket.cpp"

class UDPServer : public UDPSocket
{
private:
    sockaddr_in last_client_;

public:
    void listen()
    {
        if (bind(sockfd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
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
        last_client_ = address;
    }
};