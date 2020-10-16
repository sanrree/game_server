#include "./UDPSocket/client.cpp"
#include "./InputLooper/inputLooper.cpp"

class SocketClientController : public InputLooperDelegate
{
private:
    UDPClient &client_;
    InputLooper &looper_;

public:
    SocketClientController(UDPClient &client, InputLooper &looper)
        : client_(client), looper_(looper) {}

    void start()
    {
        std::thread t(&UDPClient::listen, &client_);
        t.detach();

        looper_.startInputLoop(BUFFER_SIZE);
    }

    void onInputSubmit(char *message)
    {
        client_.send(message);
    }
};

int main()
{
    UDPClient client = UDPClient();

    client.setListenerAddress("127.0.0.1", 8081);
    client.setFlag(NON_BLOCKING);

    InputLooper looper = InputLooper();

    SocketClientController scc = SocketClientController(client, looper);

    looper.delegate = &scc;

    scc.start();

    return 0;
}