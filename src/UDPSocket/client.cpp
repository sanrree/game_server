#include "socket.cpp"

class UDPClient : public UDPSocket
{
public:
    void onMessageReceive(char *message, sockaddr_in address)
    {
        std::cout << message;
    }

    void send(char *message)
    {
        strcat(message, "\n");
        sendMessage(message, listen_addr);
    }
};