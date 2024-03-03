/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/02 09:04:06 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(ConfigFile& config ) : req(config), res(req), stage(REQLINE)
{
}

Client::~Client()
{
}

Client::Client(const Client& copy) : req(copy.req), res(req), stage(copy.stage)
{
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
	// if (!res.getBody().empty())
	// 	res.setBody(res.getBody() + reqBuff);
	// else
	// 	res.setBody(reqBuff);
	// reqBuff.clear();
	sendResponse();
} 
