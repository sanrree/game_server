#include "./GameEventHandler.h"
#include "string.h"
#include <iostream>

GameEventHandler::GameEventHandler(TCPServer *server)
{
    server_ = server;
    server_->delegate = this;
}

void GameEventHandler::onDataReceive(byte *message, int clientfd)
{
    Request request;

    byte payload[sizeof(message) - 1];

    parseData(message, payload, &request);

    std::cout << (int)request << std::endl;

    if (delegate == nullptr)
    {
        return;
    }

    switch (request)
    {
    case Request::JOIN_GAME:
        (*delegate).onGameRequest(clientfd);
        break;
    case Request::END_GAME:
        (*delegate).onEndGame(clientfd);
        break;
    case Request::SEND_DATA:
        (*delegate).onNewData(clientfd, payload);
        break;
    }
}

void GameEventHandler::parseData(byte *buffer, byte *payload, Request *action)
{
    int length = sizeof(buffer) - 1;

    memcpy(payload, buffer + 1, length);

    *action = (Request)buffer[0];
}

void GameEventHandler::sendData(int clientfd, Response action)
{
    byte buffer[1]{(unsigned char)action};

    if (server_ != nullptr)
    {
        server_->send(clientfd, buffer);
    }
}

void GameEventHandler::sendData(int clientfd, Response action, byte *payload)
{
    std::cout << "Sending data to: " << clientfd << std::endl;

    int length = sizeof(payload);
    byte buffer[length + 1];

    memcpy(buffer + 1, payload, length);

    buffer[0] = (unsigned char)action;

    if (server_ != nullptr)
    {
        server_->send(clientfd, buffer);
    }
}
