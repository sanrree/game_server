#include "../UDPSocket/server.cpp"
#include "../InputLooper/inputLooper.cpp"

class ServerController : public InputLooperDelegate
{
private:
    UDPServer &server_;

public:
    ServerController(UDPServer &server) : server_(server)
    {
    }

    void start()
    {
        std::thread t(&UDPServer::listen, &server_);
        t.detach();
    }
};