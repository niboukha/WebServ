/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/27 22:06:13 by niboukha         ###   ########.fr       */
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
	std::string	s;
	
	try
	{

		while (stage != REQBODY and reqBuff.find("\r\n") != std::string::npos)
			req.parseRequest(reqBuff, stage);
		
	}
	catch(Request::BadRequest& bReq)
	{

		s = bReq.getPairCodePath().first;
		res.setStatusCodeMsg(s);//mochkil hna !!!
		res.setPath(bReq.getPairCodePath().second);
		stage = RESHEADER;
	}
	catch(Request::NotImplemented& NotImplemented)
	{
		s = NotImplemented.getPairCodePath().first;
		res.setStatusCodeMsg(s);
		res.setPath(NotImplemented.getPairCodePath().second);
		stage = RESHEADER;
	}
}

void	Client::sendResponse()
{
	std::string	s;
	int			i;
	
	i = 0;
	while (stage != RESEND)
	{
		try
		{
			stage = res.sendResponse(stage);
			if (stage == REQBODY)
				break;
			if (stage == RESBODY && i == 0)
			{
				i = 1;
				std::cout << res.getPath() << "\n";
				std::cout << res.getHeaderRes() << "\n";
				send(newSockFd, res.getHeaderRes().c_str(), 
				strlen(res.getHeaderRes().c_str()), 0);
			}
		}
		catch (std::string path)
		{
			res.setPath(path);
			// std::cout << res.getPath() << "\n";
			stage = RESHEADER;
		}
	}
	if (stage == RESEND && i == 1)
	{
		i = 2;
		s = res.getPath();
		// std::cout << res.getBodyRes() << "\n";
		send(newSockFd, res.getBodyRes().c_str(), 
		Utils::stringToLong(res.getContentLength(s)), 0);
	}
}

void	Client::serve()
{
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	char	buffer[1000];
	
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
	int b;
	int	fd;
	int i = 0;
	fd =  open("extraFiles/infile", O_RDWR | O_TRUNC);
	while ( (b = read(newSockFd, buffer, 1000)))
	{
		i += b;
		// std::cout << "------> b = " << b << " " << i << std::endl;
		write(fd, buffer, b);
		if (i == 134481)
			break;
	}
	
	// int b = read(newSockFd, buffer, 1000);

	char    buff[BUF];
	// int		fd;
	int		byte;

	// write(fd, buffer, b);
	// close(fd);
	fd =  open("extraFiles/infile", O_RDWR);

	while ((byte = read(fd, buff, BUF)))
	{
		reqBuff += std::string(buff, byte);//t9der  dir mochkil !!!
		recieveRequest();
		if (stage != REQLINE && stage != REQHEADER)
		{
			res.setBody(reqBuff);
			reqBuff.clear();
			sendResponse();
		}
	}
	close(newSockFd);
	// close(sockFd);
} 
