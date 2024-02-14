/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:42:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/14 13:42:09 by niboukha         ###   ########.fr       */
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

const mapStrVect    Request::getLocationMethod() const
{
    return ( locationMethod );
}

const std::string   Request::getPathHeader( ) const
{
    return ( pathHeader );
}

const std::string   Request::getProtocolVersion( ) const
{
    return ( protocolVersion );
}

const std::map<std::string, std::string>&	Request::getErrorPage( ) const
{
    return ( errorPages );
}

// void	Request::parseRequest(char *req )
// {
//     //if the parsing of the request is finished the stage turn to RESHEADER
//     //know i should go to the response, how?? maerftch (soaooaoaoaooaooad tfo elk)
// }

