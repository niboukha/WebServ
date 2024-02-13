/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:20 by shicham           #+#    #+#             */
/*   Updated: 2024/02/12 10:45:04 by niboukha         ###   ########.fr       */
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

const   mapStrVect& Server::getServerData() const
{
    return serverData;
}

bool Server::serverValidDirective(std::string directive)
{
    if (directive.compare("host") and directive.compare("port") and directive.compare("error_page")
           and directive.compare("client_max_body_size") and directive.compare("server_namet"))
        return true;
    throw UnknownDirective();
}
