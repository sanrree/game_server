#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define NON_BLOCKING O_NONBLOCK
#define BUFFER_SIZE 1024

#define byte unsigned char

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

class SocketReceiverDelegate
{
public:
    virtual void onMessageReceive(byte *message, sockaddr_in fromAddress) = 0;
};