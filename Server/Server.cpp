/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:20 by shicham           #+#    #+#             */
/*   Updated: 2024/03/17 12:49:14 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

bool     (*Server::functionsLocation[])(std::vector<std::string> &) = {&Server::isValidRoot, &Server::isValidAutoIndex, 
                                                        &Server::isValidUploadPass, &Server::isValidCgiPass, &Server::isValidReturn};//update
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

const int&  Server::getMasterSocket() const
{
    return masterSocket;
}

void    Server::createAndBindSocket(fd_set& readFds)
{
    struct addrinfo hints;
    struct addrinfo *rslt;
    int     opt;
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_addrlen = sizeof(sockaddr_in);
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;
    hints.ai_addr = NULL;
    int status = getaddrinfo(getServerData().find("host")->second.c_str(), \
    getServerData().find("port")->second.c_str(), &hints, &rslt);
    if( status < 0)
        throw gai_strerror(status);
    for (struct addrinfo * rp = rslt; rp != NULL; rp = rp->ai_next)
    {
        masterSocket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
       
        if (masterSocket < 0)
            continue;
        opt = 1;
        if (setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        {
            freeaddrinfo(rslt);
            throw strerror(errno);
        }
        if (bind(masterSocket, rp->ai_addr, rp->ai_addrlen) < 0)
        {
            freeaddrinfo(rslt);
            throw strerror(errno);
        }
        break;
       
    }
    FD_SET(masterSocket, &readFds);
    freeaddrinfo(rslt);
    if (listen(masterSocket, SOMAXCONN) < 0)
        throw strerror(errno);
}

int   Server::acceptNewConnection(fd_set& readFds, fd_set& writeFds)
{
    sockaddr_in clientAdd;
    socklen_t   clientAddLen;
    int         acceptedClient;
    struct  in_addr  addr;
    uint32_t ip_address ;
    char    buffAdd[10];

    clientAddLen = sizeof(clientAdd);
    if((acceptedClient  = accept(masterSocket, (struct sockaddr*)&clientAdd, &clientAddLen)) < 0)
        throw strerror(errno);
    
    addr = clientAdd.sin_addr;
    ip_address = addr.s_addr;
   
    sprintf(buffAdd, "%u.%u.%u.%u", (ip_address & 0xFF), 
        ((ip_address >> 8) & 0xFF),((ip_address >> 16) & 0xFF),((ip_address >> 24) & 0xFF));
        
    clientIp = std::string(buffAdd);
    
    FD_SET(acceptedClient, &readFds);
    FD_SET(acceptedClient, &writeFds);
    return acceptedClient;
}

const std::string& Server::getClientIp() const
{
    return clientIp;
}

bool Server::serverValidDirective(std::string &directive, std::string& value)//update
{
    std::string    directives[3] = {"host", "error_page", "server_name"};

    if (!directive.compare("client_max_body_size"))
        return (isValidClientMaxBodySize(value));
    if (!directive.compare("port"))
        return (isValidPort(value));
    for(size_t i = 0; i < 3; i++)
    {
        if (!directives[i].compare(directive))
            return true;
    }
    throw UnknownServerDirective();
}

bool    Server::isValidClientMaxBodySize(std::string &ClientMaxBodySizeValue)//update
{
    std::istringstream  iss(ClientMaxBodySizeValue);
    long long           ClientMaxBodySize;
    std::string            strLongLong;
    char                remain;
    std::ostringstream oss;
 
    if (!(iss >> ClientMaxBodySize) or (iss >> remain) or (ClientMaxBodySize < 0))
        throw InvalidDirectiveArgument();
    oss << ClientMaxBodySize;
    strLongLong = oss.str();
    if (strLongLong.compare(ClientMaxBodySizeValue))
        throw InvalidDirectiveArgument();
    return true;
}

bool  Server::isValidPort(std::string &portValue)
{
    std::istringstream  iss(portValue);
    long long           port;
    std::string            strLongLong;
    char                remain;
    std::ostringstream oss;
 
    if (!(iss >> port) or (iss >> remain) or (port < 0))
        throw InvalidDirectiveArgument();
    oss << port;
    strLongLong = oss.str();
    if (strLongLong.compare(portValue) or port > 65535)
        throw ("Config file: port out of range ");
    return true;
}


bool    Server::locationValidDirective(std::string &directive, std::vector<std::string> &values)//update
{
    std::string locDirectives[5] = {"root", "autoindex", "upload_pass", 
                                     "cgi_pass", "return"};

    if (!directive.compare("index") 
        or !directive.compare("allow_methodes"))
        return true;
    for(size_t i = 0; i < 5; i++)
    {
        if (!locDirectives[i].compare(directive))
        {
            functionsLocation[i](values);
            return true;
        }
    }
    throw  UnknownLocationDirective();
}

bool    Server::isValidRoot(std::vector<std::string> &rootValue)
{
    if (rootValue.size() != 1) 
        throw InvalidNumberOfArguments();
    else if ((rootValue[0].find_last_of("/") != rootValue[0].length() - 1))
        throw("config file : invalid root directive");
    return true;
}

bool    Server::isValidAutoIndex(std::vector<std::string> &autoindexValue)
{
     if (autoindexValue.size() != 1)
        throw InvalidNumberOfArguments();
    else if (autoindexValue.size() == 1 
        and (!autoindexValue[0].compare("on") 
        or !autoindexValue[0].compare("off")) )
       return true;
    throw InvalidDirectiveArgument();
}

bool        Server::isValidUploadPass(std::vector<std::string> &uploadValue)
{
     if (uploadValue.size() != 1 )
        throw InvalidDirectiveArgument();
      else if(uploadValue[0].find_first_of("/"))
        throw ("config file : invalid upload_pass directive");
    return true ;
}

bool    Server::isValidCgiPass(std::vector<std::string> &cgiValue)
{
    if (cgiValue.size() != 1 )
       throw InvalidNumberOfArguments();
    return true;
}

bool  Server::isValidReturn(std::vector<std::string> &returnValue)
{
    if (returnValue.size() != 2 or returnValue[0].compare("301"))
          throw InvalidDirectiveArgument();
    return true;
}

bool    Server::serverObligatoryDirectives()
{
    std::string serverDirectives[3] = {"host", "port", "client_max_body_size"};
    for (size_t i = 0; i < 3; i++)
    {
        if (serverData.find(serverDirectives[i]) == serverData.end())
            throw ("config File : directive " + serverDirectives[i] +" required in server block");   
    }
    return true;
}
