#include "./TCPListener/TCPListener.cpp"

int main()
{
	TCPListener server = TCPListener();

	server.start(8081);

	return 0;
}
