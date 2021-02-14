#include "./TCPServer/TCPServer.cpp"
#include "./Game/GameServer.cpp"

int main()
{
	TCPServer server = TCPServer();

	GameServer gameServer = GameServer(&server);

	server.start(8081);

	return 0;
}
