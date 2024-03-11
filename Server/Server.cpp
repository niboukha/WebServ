/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:20 by shicham           #+#    #+#             */
/*   Updated: 2024/03/11 19:10:07 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"

bool     (*Server::functionsServer[])(std::string &) = {&Server::isValidHost, &Server::isValidPort, 
                                                &Server::isValidErrorPage, 
                                                &Server::isValidClientMaxBodySize};//update

bool     (*Server::functionsLocation[])(std::vector<std::string> &) = {&Server::isValidRoot, &Server::isValidAutoIndex, 
                                                        &Server::isValidUploadPass};//update
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
    if(getaddrinfo(getServerData().find("host")->second.c_str(), \
    getServerData().find("port")->second.c_str(), &hints, &rslt) < 0)
        throw strerror(errno);
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

    clientAddLen = sizeof(clientAdd);
    if((acceptedClient  = accept(masterSocket, (struct sockaddr*)&clientAdd, &clientAddLen)) < 0)
        throw strerror(errno);
    FD_SET(acceptedClient, &readFds);
    FD_SET(acceptedClient, &writeFds);
    return acceptedClient;
}
 
bool Server::serverValidDirective(std::string &directive, std::string& value)//update
{
    std::string    directives[4] = {"host", "port", "error_page", "client_max_body_size"};

    if (!directive.compare("server_name"))
        return true;
    for(size_t i = 0; i < 4; i++)
    {
        if (!directives[i].compare(directive))
        {
            functionsServer[i](value);
            return true;
        }
    }
    throw UnknownServerDirective();
}

bool   Server::isValidHost(std::string &hostValue)//update
{
    struct in_addr addr;
    
    if (!hostValue.compare("localhost") 
    or inet_aton(hostValue.c_str(), &addr))//to avoid
        return true;
    throw InvalidDirectiveArgument();
}

bool Server::isValidPort(std::string &portValue)//update
{
    std::istringstream iss(portValue);
    int port;
    char remain;
    
    if (!(iss >> port) or (iss >> remain) or port < 0 or port > 65536)
        throw InvalidDirectiveArgument();
    return true;
}

bool    Server::isValidErrorPage(std::string &errorPageValue)//update
{
    std::istringstream iss(errorPageValue);
    int error;
    char remain;
    
    if (!(iss >> error) or (iss >> remain))//to chek mn b3ed 
        throw InvalidDirectiveArgument();
    return true;
}

bool    Server::isValidClientMaxBodySize(std::string &ClientMaxBodySizeValue)//update
{
    std::istringstream  iss(ClientMaxBodySizeValue);
    long long           ClientMaxBodySize;
    char                remain;
 
    if (!(iss >> ClientMaxBodySize) or (iss >> remain) or (ClientMaxBodySize < 0))
        throw InvalidDirectiveArgument();
    return true;
}

bool    Server::locationValidDirective(std::string &directive, std::vector<std::string> &values)//update
{
    std::string locDirectives[3] = {"root", "autoindex", "upload_pass"};

    if (!directive.compare("index") 
        or !directive.compare("allow_methodes")
        or !directive.compare("cgi_pass")
        or !directive.compare("return"))
    {
        std::vector <std::string> vect;
        
        vect.push_back("");
        if (!directive.compare("index") and !values.size())             values = vect;//update 
        if (!directive.compare("allow_methodes") and !values.size())    values = vect;//update
        if (!directive.compare("cgi_pass") and !values.size())          values = vect;//update 
         if (!directive.compare("return") and !values.size())           values = vect;;//update
        return true;
    }
    for(size_t i = 0; i < 3; i++)
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
    if (rootValue.size() > 1)//to check
       throw InvalidNumberOfArguments();
    return true;
}

bool    Server::isValidAutoIndex(std::vector<std::string> &autoindexValue)
{
    if (!autoindexValue.size())
    {
        autoindexValue.push_back("");
        return true;
    }
    else if (autoindexValue.size() > 1)
        throw InvalidNumberOfArguments();
    else if (autoindexValue.size() == 1 
        and (!autoindexValue[0].compare("on") 
        or !autoindexValue[0].compare("off")) )
       return true;
    throw InvalidDirectiveArgument();
}

bool        Server::isValidUploadPass(std::vector<std::string> &uploadValue)
{
     if (uploadValue.size() > 1)
        throw InvalidNumberOfArguments();
    return true ;
}

bool    Server::serverObligatoryDirectives()
{
    std::string serverDirectives[4] = {"host", "port", "server_name", "client_max_body_size"};
    
    for (size_t i = 0; i < 4; i++)
    {
        if (serverData.find(serverDirectives[i]) == serverData.end())
            throw ("config File : directive " + serverDirectives[i] +" required in server block");   
    }
    return true;
}

bool    Server::locationObligatoryDirectives(std::map<std::string, std::string> &loc)//i didn't use it 
{
    std::string locDirectives[4] = {"root", "index", "autoindex", "allow_methodes"};

     for (size_t i = 0; i < 4; i++)
    {
        if (loc.find(locDirectives[i]) == loc.end())
            throw MissingLocationDirectives();   
    }
    return true;
}
