#include "./UDPSocket/server.cpp"

int main()
{
	UDPServer().start("127.0.0.1", 8081);

	return 0;
}