/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:16:09 by shicham           #+#    #+#             */
/*   Updated: 2024/02/23 11:56:58 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request()
{
}

Request::~Request()
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

const std::string   Request::getRequestedPath( ) const
{
    return ( requestedPath );
}

const std::string   Request::getProtocolVersion( ) const
{
    return ( protocolVersion );
}

const	std::map<std::string, std::string>	Request::getServer( ) const
{
	return (server);
}

const	std::map<std::string, std::string>	Request::getHeaders() const
{
	return (headers);
}


void   Request::parseRequest(std::string &buff, Stage &stage)
{
    if (stage == REQLINE)
    {
        parseRequestLine(buff);
        parseUri();
        stage =  REQHEADER;
    }
    else if (stage == REQHEADER)
    {
        parseHeader(buff);
        if (!buff.find("\r\n"))
        {
            matchingLocation();
            // std::cout <<"----> location :" << std::endl;
            // for (mapStrVect::iterator i = location.begin(); i != location.end(); i++)
            // {
            //     std::cout << "key |" << i->first << "| " << "value |" << i->second[0] << "|" << std::endl;
            // }
            
            stage = REQBODY;
        }
    }
    
}

std::string Request::decodePercentEncoded(std::string hexastr)
{
    int decimalValue;
    std::string toString;
    
    std::istringstream(hexastr) >> std::hex >> decimalValue;
    toString.append(1, decimalValue);
    return toString;
}

void    Request::decodeUri()
{
    // std::cout << "=======> decode uri : " << std::endl;
    for (size_t i = 0; i < uri.size(); i++)
    {
        if ((i != uri.size() - 3) and 
        ((uri[i] == '%' and isdigit(uri[i + 1]) and isupper(uri[i + 2]))
        or (uri[i] == '%' and isdigit(uri[i + 1]) and isdigit(uri[i + 2]))))
            uri.replace(i, 3, decodePercentEncoded(uri.substr(i + 1, 2)));
    }
    // std::cout << "=========> the end of decoding uri : " << std::endl;
}

void   Request::parseUri()
{
    size_t      autorityEnd, schemeEnd;
    
    decodeUri();
    // uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%");check if uri is valid
    schemeEnd = uri.find(":");
    if (schemeEnd != std::string::npos)
    {   
        scheme = uri.substr(0, schemeEnd);
        autorityEnd = uri.find("/", schemeEnd + 2);
        autority = uri.substr(schemeEnd, autorityEnd);
        requestedPath = uri.substr(autorityEnd, uri.find("?", autorityEnd));
        queryParameters = uri.substr(uri.find("?", autorityEnd));
    }
    requestedPath = uri.substr(uri.find("/"));
}

void    Request::parseRequestLine(std::string    &buff)
{
    std::vector<std::string>    splitReqLine;
    std::string                 reqLine;
    
    reqLine = buff.substr(0 ,buff.find("\r\n"));
    buff = buff.substr(buff.find("\r\n") + 2);
    splitReqLine = StringOperations::split(reqLine, " ");
    // if (splitReqLine.size() != 3)//to add
    method = splitReqLine[0];
    uri =splitReqLine[1];
    protocolVersion = splitReqLine[2];
}

void    Request::parseHeader(std::string &buff)
{
    std::string key, header, value;
    size_t      found;

    found = buff.find("\r\n");
    // if (found  == std::string::npos)
    header = buff.substr(0, found);
    buff = buff.substr(found + 2);
    key = header.substr(0, header.find_first_of(":"));
    for (size_t i = 0; i < key.length(); i++)
        key[i] =  tolower(key[i]);
    value = StringOperations::trim(header.substr(key.length() + 1));
    headers[key] = value;
}

void    Request::SetConfigFile(ConfigFile& configFile)
{
    configFileData = configFile;
}

size_t  Request::longestMatchingLocation(const std::string& prefix)
{
    size_t i = 0;
    for (; i < uri.size() and uri[i] == prefix[i]; i++)
    {}
    if (prefix[i - 1] == '/') //should check after
        return i;
    return 0;
}

void    Request::matchingLocation()
{
    Server  matchServer;
    size_t  longestOne, sizeMatching;

    matchServer = findServer();
    // std::cout << "matching server : " << matchServer.getServerData().at("host")<< std::endl;
    // for (size_t i = 0; i < ; i++)
    // {
    //     /* code */
    // }
    
    server = matchServer.getServerData();
    // std::cout << "server copy : " << server.at("host") << std::endl;
    longestOne = 0;
    for (std::map<std::string, mapStrVect>::const_iterator 
        i = matchServer.getLocations().begin();
        i != matchServer.getLocations().end(); i++)
    {
        sizeMatching = longestMatchingLocation((i->first));
        if (longestOne < sizeMatching)
        {
            location = i->second;  
            longestOne = sizeMatching;
        }
    } 
}

Server  Request::findServer()
{
    std::vector<Server> servers;
    std::pair<std::string, std::string> pair;

    servers = configFileData.getServers();
    pair.first = StringOperations::split(headers["host"], ":")[0];
    pair.second = StringOperations::split(headers["host"], ":")[1];
   for (size_t i = 0; i < servers.size(); i++)
   {
        if (!servers[i].getServerData().at("host").compare(pair.first) 
            and !servers[i].getServerData().at("port").compare(pair.second))//how can i known wich server
            return servers[i];
   }
    return servers[0];
}
