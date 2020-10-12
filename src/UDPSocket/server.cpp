#include "socket.cpp"
#include "inputLooper.cpp"

class UDPServer : UDPSocket, InputLooperDelegate
{
public:
    void start(const char *address, int port)
    {
        recv_addr.sin_family = AF_INET;
        recv_addr.sin_addr.s_addr = inet_addr(address);
        recv_addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0)
            error("Socket binding failed");

        if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
        {
            error("fcntl call error");
        }

        startInputListenerAsync();

        InputLooper looper = InputLooper();
        looper.delegate = this;
    }

    void onInputSubmit(char* message){
        
    }
};