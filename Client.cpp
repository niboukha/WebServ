/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/21 21:32:15 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Client::Client() : res(req), stage(REQLINE)
// {
// }

Client::Client( Request& request) : req(request) , stage(REQLINE)
{
}

Client::~Client()
{
}

const std::string&	Client::getRecievedReq() const
{
	return (recievedReq);
}

const Stage&	Client::getStage( ) const
{
	return ( stage );
}

void	Client::recieveRequest()
{
	while (reqBuff.find("\n") != std::string::npos)
		req.parseRequest(reqBuff, stage);
}

void	Client::sendResponse()
{
	// while (1)
	// {
	// 	try
	// 	{
	// 		stage = res.sendResponse(stage);
	// 		if (stage == RESEND)
	// 			break;
	// 	}
	// 	catch (std::string path)
	// 	{
	// 		stage = RESHEADER;
	// 	}
	// }
}

void	Client::serve()
{
	int 	sockFd = socket(AF_INET, SOCK_STREAM, 0), newSockFd;
	char	buffer[1024];
	
    if (sockFd < 0)
        std::cout << "failed to create socket" << std::endl;
		
    struct sockaddr_in	serverAdd, clientAddr;
    socklen_t 			clientAddrLen;
	
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_addr.s_addr = INADDR_ANY;
    serverAdd.sin_port = htons(PORT);
	
    int opt = 1;
    if (setsockopt(sockFd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)) < 0)
	{
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
	
    if (bind(sockFd, (struct sockaddr*) &serverAdd, sizeof(serverAdd)) < 0)
	{
        perror("bind");
	}
		
	listen(sockFd, 5);
		
		// perror("listen");
	clientAddrLen =  sizeof(clientAddr);
	newSockFd = accept(sockFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	
	if (newSockFd < 0)
		perror("accept");
	read(newSockFd, buffer, 1024);
	
	std::cout << "the message recieved : " << std::endl;
	std::cout << buffer << std::endl;

	// char    buff[51];
	// int		fd;
	// fd =  open("infile", O_RDWR);
	// int		bytes;
	// write(fd, buffer, 1024);
	// close(fd);
	// fd =  open("infile", O_RDWR);
	// while ((bytes = read(fd, buff, 50)))
	// {
	// 	std::cout << "dkhel\n";
	// 	buff[50] = '\0';
	// 	std::cout << "buff : " << buff << std::endl;
	// 	// std::cout << "buff length : " << strlen(buff) << std::endl;
	// 	// std::cout << "bytes : " << bytes << std::endl;
	// 	reqBuff += buff;//don't use append 
	// 	std::cout << "heeere " << std::endl;
	// 	recieveRequest();
	// }
	close(sockFd);
}


