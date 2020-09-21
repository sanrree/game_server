#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <thread>

#define PORT_NO 8000
#define BUFFER_SIZE 1024

void error(const char* msg){
	perror(msg);
	exit(1);
}

void listen_input(char* str){
	while(1){
		std::cin >> str;
		std::cout<< str;
	}
}

int main(){
    int sockfd, newsockfd;

	struct sockaddr_in serv_addr, client_addr;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		error("Socket creation failed");
		return 0;
	}

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

 	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT_NO);

    if(bind(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
    	error("Socket binding failed");
    }

    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    listen(sockfd, 1);

    int n;

    int buffersize = 1024;
	char buffer[BUFFER_SIZE];

	socklen_t len = (int)sizeof(client_addr);

	char message[BUFFER_SIZE];

	std::thread t(listen_input, message);
	t.detach();

    while(1){

    	n = recvfrom(sockfd, (char *)buffer, buffersize, MSG_WAITALL, (struct sockaddr*)&client_addr, &len);

		if(n > 0){
			std::cout << buffer << std::endl;
    	}

    	if(message[0] == '\0'){
				continue;
		}
			std::cout << message << std::endl;

    		sendto(sockfd, message, strlen(message),  
        	MSG_CONFIRM, (const struct sockaddr *) &client_addr, len); 
    		
			memset(&buffer, 0, buffersize);
			memset(&message, 0, buffersize);
			message[0] = '\0';

    }

	return 0;
}

