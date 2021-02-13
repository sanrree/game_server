#include "Commons.h"

class SocketReceiverDelegate
{
public:
    virtual void onMessageReceive(byte *message, sockaddr fromAddress) = 0;
};