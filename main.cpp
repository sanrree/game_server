#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>


#define PORT_NO 8000 

void error(const char* msg){
	perror(msg);
	exit(1);
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

    listen(sockfd, 1);

    int n;

    int buffersize = 1024;
	char buffer[buffersize];

	socklen_t len = (int)sizeof(client_addr);

	char hello[buffersize] = "Hello client\n"; 


    while(1){
		

    	n = recvfrom(sockfd, (char *)buffer, buffersize, MSG_WAITALL, (struct sockaddr*)&client_addr, &len);
    	
		std::cout << buffer << std::endl;

		memset(&buffer, 0, buffersize);

		std::cin >> hello;
    	sendto(sockfd, hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &client_addr, 
            len); 

    	// accep
    }



	return 0;
}