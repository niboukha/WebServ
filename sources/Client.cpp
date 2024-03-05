/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/05 09:02:57 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(std::vector<Server>& servers, int &fd) : req(servers), res(req), stage(REQLINE)
{
	this->fd = fd;
	
}

Client::~Client()
{
}

Client::Client(const Client& copy) : req(copy.req), res(req), stage(copy.stage), fd(copy.fd)
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

void	Client::setReqBuff(const std::string& buff)
{
	reqBuff = buff;
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
		req.fillErrorPages();//tmp 
		// std::cout << "in request \n";
		while (stage != REQBODY and reqBuff.find("\r\n") != std::string::npos)
			req.parseRequest(reqBuff, stage);
	}
	catch(Request::BadRequest& bReq)
	{
		// std::cout << "in bad request -> \n";
		req.setMethod("GET");
		req.setProtocolVersion("HTTP/1.1");
		s = bReq.getPairCodePath().second;

		res.setStatusCodeMsg(s);//mochkil hna !!!
		// std::cout << res.pathErrorPage(bReq.getPairCodePath().first) << "\n";
		res.setPath(res.pathErrorPage(bReq.getPairCodePath().first));

		// std::cout << res.getPath() << "\n";
		stage = REQBODY;
	}
	catch(Request::NotImplemented& NotImplemented)
	{
		req.setMethod("GET");
		req.setProtocolVersion("HTTP/1.1");
		// std::cout << "in not Implemented -> \n";
		s = NotImplemented.getPairCodePath().second;
		res.setStatusCodeMsg(s);
		res.setPath(res.pathErrorPage(NotImplemented.getPairCodePath().first));
		// std::cout << "khera -> " << res.getPath() << "\n";
		// std::cout << req.getMethod() << "\n";
		stage = REQBODY;
	}
	// catch(Request::NotSupported& NotSupported)
	// {
	// 	req.setMethod("GET");
	// 	req.setProtocolVersion("HTTP/1.1");
	// 	// std::cout << "in not SuppoNotSupported -> \n";
	// 	s = NotSupported.getPairCodePath().second;
	// 	res.setStatusCodeMsg(s);
	// 	res.setPath(res.pathErrorPage(NotSupported.getPairCodePath().first));
	// 	// std::cout << "khera -> " << res.getPath() << "\n";
	// 	// std::cout << req.getMethod() << "\n";
	// 	stage = REQBODY;
	// }
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
	// std::cout << "------> here\n";
	sendResponse();

} 
