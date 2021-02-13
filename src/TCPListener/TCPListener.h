#include "Commons.h"
#include "SocketReceiverDelegate.h"
#include <thread>

class TCPListener
{
private:
    sockaddr_in listen_addr_;

    int sockfd_;

    byte buffer_[BUFFER_SIZE];

    bool listening_;

    void handleClient(int clientfd);

    void setAddress(int address, int port);
public:
    SocketReceiverDelegate *delegate = nullptr;

    TCPListener();

    void start(int port);

    void send(const char *data, sockaddr addr);

    void setFlag(int flag);

    void stop();
};