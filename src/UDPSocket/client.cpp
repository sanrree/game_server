#include "socket.cpp"

class UDPClient : public UDPSocket
{
public:
    void onMessageReceive(byte *message, sockaddr_in address)
    {
        std::cout << message;
    }

    void send(char *message)
    {
        strcat(message, "\n");
        sendMessage(message, sizeof(message), listen_addr);
    }
};