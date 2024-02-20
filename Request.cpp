/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:42:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 13:46:38 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request ()
{
}

Request::~Request ()
{
}

const std::string  Request::getMethod( ) const
{
    return ( method );
}

const mapStrVect    Request::getLocation() const
{
    return ( location );
}

const mapStrVect	Request::getHeaders() const
{
    return ( headers );
}

const std::string   Request::getRequestedPath( ) const
{
    return ( requestedPath );
}

const std::string   Request::getProtocolVersion( ) const
{
    return ( protocolVersion );
}

const std::map<std::string, std::string>&	Request::getServer() const
{
	return (server);
}

Stage	Request::parseRequest(std::string buffer)
{
    (void)buffer;
    return (RESHEADER);
}

// void	Request::parseRequest(char *req )
// {
//     //if the parsing of the request is finished the stage turn to RESHEADER
//     //know i should go to the response, how?? maerftch (soaooaoaoaooaooad tfo elk)
// }

