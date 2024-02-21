/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:20 by shicham           #+#    #+#             */
/*   Updated: 2024/02/21 19:41:48 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{  
}

Server::~Server()
{  
}

void    Server::setServerData(std::map<std::string, std::string>& servData)
{
    serverData = servData;
}

void    Server::setLocations(std::map<std::string,  mapStrVect>& locs)
{
    locations = locs;
}

const   std::map<std::string, std::string>& Server::getServerData() const
{
    return serverData;
}

const std::map<std::string,  mapStrVect>& Server::getLocations() const
{
    return locations;
}

bool Server::serverValidDirective(std::string &directive, std::string& value)
{
    if (!directive.compare("host"))
        Server::isValidHost(value);
    else if (!directive.compare("port"))
        Server::isValidPort(value);
    else if (!directive.compare("error_page"))
        Server::isValidErrorPage(value);
    else if (!directive.compare("client_max_body_size"))
        Server::isValidClientMaxBodySize(value);
    else if (!directive.compare("server_name"))
        Server::isValidServerName(value);
    else
        throw UnknownServerDirective();
    return true;
}

bool   Server::isValidHost(std::string &hostValue)
{
    int a, b, c, d;
    
 //should check
    if (!hostValue.compare("localhost"))
        return true;
    else if (sscanf(hostValue.c_str(),"%d.%d.%d.%d", &a, &b, &c, &d) != 4
    or a <= 0 or a >= 255 or  b <= 0 or b >= 255 or c <= 0 
    or c >= 255 or  d <= 0 or d >= 255)
        throw InvalidDirectiveArgument();
    return true;
}

bool Server::isValidPort(std::string &portValue)
{
    int port;
    
    if (portValue.find_first_not_of("0123456789") != std::string::npos
    or sscanf(portValue.c_str(),"%d", &port) != 1 or port < 0 or port > 65536)
        throw InvalidDirectiveArgument();
    return true;
}

bool    Server::isValidErrorPage(std::string &errorPageValue)
{
    int error;
    
    if (errorPageValue.find_first_not_of("0123456789") != std::string::npos
    or sscanf(errorPageValue.c_str(),"%d", &error) != 1)
        throw InvalidDirectiveArgument();
    return true;
}

bool    Server::isValidClientMaxBodySize(std::string &ClientMaxBodySizeValue)
{
    int ClientMaxBodySize;
 
 if ( ClientMaxBodySizeValue.find_first_not_of("0123456789") != std::string::npos
    or sscanf(ClientMaxBodySizeValue.c_str(), "%d", &ClientMaxBodySize) != 1 
    or ClientMaxBodySize < 0)
        throw InvalidDirectiveArgument();
    return true;
}

bool    Server::locationValidDirective(std::string &directive, std::vector<std::string> &values)
{
    if (!directive.compare("root"))
        Server::isValidRoot(values);
    else if (!directive.compare("allow_methodes"))
        Server::isValidAllowMethodes(values);
    else if (!directive.compare("autoindex"))
        Server::isValidAutoIndex(values);
    else if (!directive.compare("index"))
        Server::isValidIndex(values);
    else if (!directive.compare("upload_pass"))
        isValidUploadPass(values);
    else 
        throw   UnknownLocationDirective();
    return true;
}

bool    Server::isValidUploadPass(std::vector<std::string> &uploadValue)
{
    if (uploadValue.size() != 1)
         throw InvalidDirectiveArgument();
    return true;
}
bool    Server::isValidServerName(std::string &serverNameValue)
{
    (void) serverNameValue;
    // if (serverNameValues.size() != 1)
    //      throw InvalidNumberOfArguments();
    return true;     
}

bool    Server::isValidRoot(std::vector<std::string> &rootValue)
{
    if (rootValue.size() != 1)
       throw InvalidNumberOfArguments();
    return true;
}

bool    Server::isValidAllowMethodes(std::vector<std::string> &allowMethodesValue)
{
    //should check
     if (!allowMethodesValue.size())
       throw InvalidNumberOfArguments();
    return true;
}

bool    Server::isValidIndex(std::vector<std::string> &indexValue)
{
     if (!indexValue.size())
        throw InvalidNumberOfArguments();
    return true;
}

bool    Server::isValidAutoIndex(std::vector<std::string> &autoindexValue)
{
    if (autoindexValue.size() != 1)
        throw InvalidNumberOfArguments();
    else if (!autoindexValue[0].compare("on") or !autoindexValue[0].compare("off") )
       return true;
    throw InvalidDirectiveArgument();
}

bool    Server::serverObligatoryDirectives()
{
    std::string serverDirectives[4] = {"host", "port", "server_name", "client_max_body_size"};
    
    for (size_t i = 0; i < 4; i++)
    {
        if (serverData.find(serverDirectives[i]) == serverData.end())
            throw MissingServerDirectives();   
    }
    return true;
}