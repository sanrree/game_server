#include "./GameServer.h"

GameServer::GameServer(TCPServer *server)
{
    GameEventHandler *eventHandler = new GameEventHandler(server);

    (*eventHandler).delegate = this;

    eventHandler_ = eventHandler;
}

GameServer::~GameServer()
{
    delete eventHandler_;
}

void GameServer::onGameRequest(int client)
{
    if (hasClientRoom(client))
    {
        return;
    }

    int pendingClient = findPendingRoom();

    if (pendingClient == 0)
    {
        std::cout << client << " waiting for players" << std::endl;

        rooms_.insert({client, 0});
        eventHandler_->sendData(client, Response::WAIT_FOR_PLAYER);

        return;
    }

    rooms_[pendingClient] = client;

    std::cout << "game started" << pendingClient << " : " << client << std::endl;

    eventHandler_->sendData(pendingClient, Response::START_GAME);
    eventHandler_->sendData(client, Response::START_GAME);
}

void GameServer::onEndGame(int client)
{
    if (int pair = findPair(client))
    {
        eventHandler_->sendData(pair, Response::END_GAME);

        rooms_.erase(pair);
        rooms_.erase(client);
    }
}

void GameServer::onNewData(int client, byte *data)
{

    int pair = findPair(client);

    std::cout << "send data to: " << pair << std::endl;

    if (pair != 0)
    {
        eventHandler_->sendData(pair, Response::GET_DATA, data);
    }
}

int GameServer::findPendingRoom()
{
    for (const auto &p : rooms_)
    {
        if (p.first > 0 && p.second == 0)
        {
            return p.first;
        }
    }

    return 0;
}

int GameServer::findPair(int client)
{
    for (const auto &p : rooms_)
    {
        if (p.second == client)
        {
            return p.first;
        }
        else if (p.first == client)
        {
            return p.second;
        }
    }

    return 0;
}

bool GameServer::hasClientRoom(int client)
{
    for (const auto &p : rooms_)
    {
        if (p.second == client || p.first == client)
        {
            return true;
        }
    }

    return false;
}