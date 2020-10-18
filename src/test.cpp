#include "./UDPSocket/client.cpp"
#include "./InputLooper/inputLooper.cpp"
#include <chrono>

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

        while (1)
        {
            std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch());

            char *time = (char *)std::to_string(ms.count()).c_str();
            client_.send(time);
        }

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