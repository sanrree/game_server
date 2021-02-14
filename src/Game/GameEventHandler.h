#include "Commons.h"
#include "../TCPServer/TCPServer.h"
#include "./GameEventReceiverDelegate.h"

class GameEventHandler : public SocketReceiverDelegate
{
public:
    GameEventReceiverDelegate *delegate = nullptr;

    GameEventHandler(TCPServer *server);

    void sendData(int clientfd, Response action);
    void sendData(int clientfd, Response action, byte* payload);

private:
    TCPServer *server_ = nullptr;

    void onDataReceive(byte *message, int clientfd);
    void parseData(byte *buffer, byte *payload, Request *action);
    
};