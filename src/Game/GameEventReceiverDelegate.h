class GameEventReceiverDelegate
{
public:
    virtual void onGameRequest(int client) = 0;
    virtual void onNewData(int client, unsigned char *data) = 0;
    virtual void onEndGame(int client) = 0;
};