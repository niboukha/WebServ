/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/23 11:52:35 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client( Request& request ) : req(request), res(req), stage(REQLINE)
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
	// try
	// {
		while (stage != REQBODY and reqBuff.find("\r\n") != std::string::npos)
			req.parseRequest(reqBuff, stage);
	// }
	// catch(std::string s)
	// {
	// 	res.setStatusCodeMsg()
	// 	res.setPath()
	// 	stage = RESHEADER;
	// }
	
	
}

void	Client::sendResponse()
{
	int i = 0;
	while (stage != RESEND)
	{
		try
		{
			stage = res.sendResponse(stage);
			if (stage == RESBODY && i == 0)
			{
				i = 1;
				std::cout <<"--------> " <<res.getHeaderRes() << std::endl;
				send(newSockFd, res.getHeaderRes().c_str(), 
				strlen(res.getHeaderRes().c_str()), 0);
			}
		}
		catch (std::string path)
		{
			res.setPath(path);
			stage = RESHEADER;
		}
	}
	std::cout <<"--------> " <<res.getBodyRes() << std::endl;
	send(newSockFd, res.getBodyRes().c_str(), 
	strlen(res.getBodyRes().c_str()), 0);
}

void	Client::serve()
{
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
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
	listen(sockFd, 55);
		
		// perror("listen");
	clientAddrLen =  sizeof(clientAddr);
	newSockFd = accept(sockFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (newSockFd < 0)
		perror("accept");
	int b = read(newSockFd, buffer, 1024);

	char    buff[51];
	int		fd;
	int		bytes;
	
	fd =  open("infile", O_RDWR);
	write(fd, buffer, b);
	close(fd);
	fd =  open("infile", O_RDWR);
	
	while ((bytes = read(fd, buff, 50)))
	{
		buff[bytes] = '\0';
		reqBuff += buff;//don't use append
		recieveRequest();
		if (stage != REQLINE && stage != REQHEADER)
			sendResponse();
		// std::cout << "hna\n";
	}
	close(newSockFd);
	close(sockFd);
} 
