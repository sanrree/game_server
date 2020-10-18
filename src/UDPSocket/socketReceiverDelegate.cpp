#include <netinet/in.h>

class SocketReceiverDelegate
{
public:
    virtual void onMessageReceive(char *message, sockaddr_in fromAddress) = 0;
};