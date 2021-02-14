#include "GameEventHandler.cpp"
#include <map>

class GameServer : public GameEventReceiverDelegate
{
private:
    std::map<int, int> rooms_;
    GameEventHandler *eventHandler_ = nullptr;

    int findPendingRoom();
    bool hasClientRoom(int client);
    int findPair(int client);

public:
    GameServer(TCPServer *server);
    ~GameServer();

    void onGameRequest(int client);
    void onNewData(int client, unsigned char *data);
    void onEndGame(int client);
};