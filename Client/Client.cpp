/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/13 14:00:08 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client/Client.hpp"

Client::Client(std::vector<Server>& servers, int &fdCopy) : req(servers), res(req), 
															stage(REQLINE), fd(fdCopy)
															, lastRead(time(0))
{
}

Client::~Client()
{
}

Client::Client(const Client& copy) : req(copy.req), res(req),
									 stage(copy.stage), fd(copy.fd),
									 lastRead(copy.lastRead)
{
}

const int&	Client::getFd() const
{
	return fd;
}

void	Client::setFd(int& fd)
{
	this->fd = fd; 
}

const Stage&	Client::getStage( ) const
{
	return ( stage );
}

const std::string& Client::getReqBuff() const
{
	return reqBuff;
}

const std::string&	Client::getSendBuff() const
{
	return sendBuff;
}

const time_t&		Client::getLastRead() const
{
	return lastRead;
}

void			Client::setLastRead(time_t& time)
{
	lastRead = time;
}

void	Client::setReqBuff(const std::string& buff)
{
	reqBuff = buff;
}

void	Client::setStage(const Stage& st)
{
	stage = st;
}
void	Client::setSendBuff(const std::string& buff)
{
	sendBuff = buff;
}


void	Client::recieveRequest()
{
	std::string	s;
	
	try
	{
		while (stage != REQBODY and reqBuff.find("\r\n") != std::string::npos)
			req.parseRequest(reqBuff, stage);
	}
	catch(std::pair<char const*, char const*>& bReq)
	{
		req.setMethod("GET");
		req.setProtocolVersion("HTTP/1.1");

		s = bReq.second;
		res.setStatusCodeMsg(s);
		res.setPath(res.pathErrorPage(bReq.first));
		stage = REQBODY;
	}
}

void	Client::sendResponse()
{
	try
	{
		stage = res.sendResponse(stage, reqBuff);
		if (stage == RESBODY)
		{
			if (!res.getHeaderRes().empty())
			{
				sendBuff = res.getHeaderRes();
				res.setHeaderRes("");
			}
			else
				sendBuff = res.getBodyRes();
		}
		else
			sendBuff.clear();
	}
	catch (std::string path)
	{
		res.setPath(path);
		stage = RESHEADER;
	}
}

void	Client::serve()
{
	if (stage < REQBODY)
		recieveRequest();
	if (stage >= REQBODY)
		sendResponse();

} 
