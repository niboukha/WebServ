/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:20 by shicham           #+#    #+#             */
/*   Updated: 2024/02/26 11:39:45 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

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

//  void     Server::addErrorPagesMissing()//update
//  {
//     if (serverData.find("404") == serverData.end())
//         serverData["404"] = "/ErrorPages/404.html";
//     if (serverData.find("501") == serverData.end())
//         serverData["501"] = "/ErrorPages/501.html";
//     if (serverData.find("400") == serverData.end())
//         serverData["400"] = "/ErrorPages/400.html";
//     if (serverData.find("414") == serverData.end())
//         serverData["414"] = "/ErrorPages/414.html";
//     if (serverData.find("413") == serverData.end())
//         serverData["413"] = "/ErrorPages/413.html";
//     if (serverData.find("405") == serverData.end())
//         serverData["405"] = "/ErrorPages/405.html";
//     if (serverData.find("403") == serverData.end())
//         serverData["403"] = "/ErrorPages/403.html";
//     if (serverData.find("201") == serverData.end())
//         serverData["201"] = "/ErrorPages/201.html";
//  }
 
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
    std::string locDirectives[4] = {"root", "autoindex", "upload_pass"};

    if (!directive.compare("index") 
        or !directive.compare("allow_methodes")
        or !directive.compare("cgi_pass"))
        return true;
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
        return true;
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
            throw MissingServerDirectives();   
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