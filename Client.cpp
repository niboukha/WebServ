/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/21 15:20:51 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : res(req), stage(REQLINE)
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

void	Client::recieveRequest(std::string buffer)
{
	stage = req.parseRequest(buffer); //should catch
}

void	Client::sendResponse()
{
	try
	{
		stage = res.sendResponse(stage);
	}
	catch (std::string path)
	{
		res.setPath(path);
		stage = RESHEADER;
	}
}

void	Client::serve()
{
	std::string	buffer;
	//buffer = recv()

	if (stage == REQHEADER)
		recieveRequest(buffer);
	else
		sendResponse();
} 
