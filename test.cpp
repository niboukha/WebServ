#include "WebServ.hpp"
#include <errno.h>

int main() {
    int sockFd = socket(AF_INET, SOCK_STREAM, 0), newSockFd;
    if (sockFd < 0)
        std::cout << "failed to create socket" << std::endl;
    struct sockaddr_in serverAdd, clientAddr;
    socklen_t clientAddrLen;
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = INADDR_ANY;
    serverAdd.sin_port = htons(PORT);
    int opt = 1;
    if (setsockopt(sockFd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (bind(sockFd, (struct sockaddr*) &serverAdd, sizeof(serverAdd)) < 0)
    {
        perror(NULL);
        std::cout << "failed to bind socket" << std::endl;
    }
	listen(sockFd, 5);
    while (1)
    {
		
		clientAddrLen =  sizeof(clientAddr);
		newSockFd = accept(sockFd,  (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (newSockFd < 0)
		std::cout << "no connection " << std::endl;
		char    buff[1024];
		recv(newSockFd, buff, 1024, 0);
		std::cout << "the message recieved from the client : " <<  newSockFd << std::endl;
		std::cout << buff << std::endl;
		close (newSockFd); 
		
    }
   if ( close(sockFd) < 0)
        perror(NULL);
    return 0;
}
