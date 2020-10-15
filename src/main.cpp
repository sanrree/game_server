#include "./UDPSocket/server.cpp"
#include "./InputLooper/inputLooper.cpp"

class ServerController : public InputLooperDelegate
{
private:
	UDPServer &server_;
	InputLooper &looper_;

public:
	ServerController(UDPServer &server, InputLooper &looper)
		: server_(server), looper_(looper)
	{
	}

	void start()
	{
		std::thread t(&UDPServer::listen, &server_);
		t.detach();

		looper_.startInputLoop(BUFFER_SIZE);
	}

	void onInputSubmit(char *message)
	{
		server_.send(message);
	}
};

int main()
{
	UDPServer server = UDPServer();

	server.setListenerAddress("127.0.0.1", 8081);
	server.setFlag(NON_BLOCKING);

	InputLooper looper = InputLooper();

	ServerController sc = ServerController(server, looper);

	looper.delegate = &sc;

	sc.start();

	return 0;
}