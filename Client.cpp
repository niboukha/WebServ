/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/14 16:48:35 by niboukha         ###   ########.fr       */
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
	try 
	{
		stage = res.sendResponse();

	}
	catch (std::string path)
	{
		//update
	}
}

void	Client::serve()
{
	if (stage == REQSTAGE)
		recieveRequest();
	else
		sendResponse();
}

