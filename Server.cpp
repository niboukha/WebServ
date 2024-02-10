/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:20 by shicham           #+#    #+#             */
/*   Updated: 2024/02/08 13:24:23 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    
}

Server::~Server()
{  
}

void    Server::setServerData(mapStrVect& servData)
{
    serverData = servData;
}

void    Server::setLocations(std::map<std::string,  Location>& locs)
{
    locations = locs;
}

const   mapStrVect& Server::getServerData() const
{
    return serverData;
}

const std::map<std::string,  Location>& Server::getLocations() const
{
    return locations;
}

bool Server::serverValidDirective(std::string directive)
{
    if (directive.compare("host") and directive.compare("port") and directive.compare("error_page")
           and directive.compare("client_max_body_size") and directive.compare("server_namet"))
        return true;
    throw UnknownDirective();
}
