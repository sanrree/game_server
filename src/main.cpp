#include <map>
#include "./UDPSocket/server.cpp"
#include "./InputLooper/inputLooper.cpp"

class IOSocket : public SocketReceiverDelegate, InputLooperDelegate
{
private:
	UDPServer &server_;

	std::map<int, sockaddr_in> clients;

	int c_id = 0;

public:
	IOSocket(UDPServer &server) : server_(server)
	{
		server_.delegate = this;
	}

	void start()
	{
		/* **TEST** */

		InputLooper looper = InputLooper();
		looper.delegate = this;

		std::thread t(&InputLooper::startInputLoop, &looper, BUFFER_SIZE);
		t.detach();

		/* **TEST** */

		server_.listen();
	}

	void broadcast(char *message)
	{
		strcat(message, "\n");

		for (auto &client : clients)
		{
			server_.sendMessage(message, client.second);
		}
	}

	void onMessageReceive(char *message, sockaddr_in address)
	{
		std::cout << message;

		bool newClient = true;

		for (auto &client : clients)
		{
			if (!UDPSocket::compareAdresses(client.second, address))
			{
				server_.sendMessage(message, client.second);
			}
			else
			{
				newClient = false;
			}
		}

		if (newClient)
		{
			std::cout << "New client added" << std::endl;
			clients.insert({++c_id, address});
		}
	}

	void onInputSubmit(char *message)
	{
		broadcast(message);
	}
};

int main()
{
	UDPServer server = UDPServer();

	server.setListenerAddress("192.168.195.132", 8081);
	server.setFlag(NON_BLOCKING);

	IOSocket sc = IOSocket(server);

	sc.start();

	return 0;
}