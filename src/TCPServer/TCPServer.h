#pragma once
#include "Commons.h"
#include "SocketReceiverDelegate.h"
#include <thread>
#include <map>
#include <mutex>

class TCPServer
{
private:
    sockaddr_in listen_addr_;

    static std::mutex mutex_;

    std::map<int, sockaddr> clients_;

    int sockfd_;

    byte buffer_[BUFFER_SIZE];

    bool listening_;

    void handleClient(int clientfd);

    void setAddress(int address, int port);

public:
    SocketReceiverDelegate *delegate = nullptr;

    TCPServer();

    void start(int port);

    void send(int client, byte *data);

    void setFlag(int flag);

    void stop();
};