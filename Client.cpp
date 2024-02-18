/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 15:27:18 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : res(req), stage(REQSTAGE)
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
	//buffer = recv()
	std::string	buffer;

	stage = req.parseRequest(buffer); //should catch
}

void	Client::sendResponse()
{
	while (1)
	{
		try
		{
			stage = res.sendResponse(stage);
			if (stage == RESEND) //
				break;
		}
		catch (std::string path)
		{
			res.setPath(path);
			stage = RESHEADER;
		}
	}
}

void	Client::serve()
{
	if (stage == REQSTAGE)
		recieveRequest();
	else
		sendResponse();
}


