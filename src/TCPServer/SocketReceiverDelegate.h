#include "Commons.h"

class SocketReceiverDelegate
{
public:
    virtual void onDataReceive(byte *message, int clientfd) = 0;
};