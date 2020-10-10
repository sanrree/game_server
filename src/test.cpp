#include "./UDPSocket/client.cpp"

int main()
{
    UDPClient().start("127.0.0.1", 8081);
   
    return 0;
}