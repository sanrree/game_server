#include "socket.cpp"

class UDPClient : UDPSocket
{
public:
    void start(const char *address, int port)
    {
        send_addr.sin_family = AF_INET;
        send_addr.sin_addr.s_addr = inet_addr(address);
        send_addr.sin_port = htons(port);

        if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
        {
            error("fcntl call error");
        }

        startInputListenerAsync();
        startInputListener();
    }
};