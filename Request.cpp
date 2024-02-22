/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:16:09 by shicham           #+#    #+#             */
/*   Updated: 2024/02/22 07:22:45 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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

const std::string   Request::getrequestedPath( ) const
{
    return ( requestedPath );
}

const std::string   Request::getProtocolVersion( ) const
{
    return ( protocolVersion );
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
        if (!buff.compare("\r\n"))
        {
            matchingLocation();
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
    
    // std::cout << "======> prseUri : " << std::endl;
    // std::cout << "---> URI : " << uri << std::endl;
    decodeUri();
    // uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%");check if uri is valid
    schemeEnd = uri.find(":");
    if (schemeEnd != std::string::npos)
    {   
        scheme = uri.substr(0, schemeEnd);
        // std::cout << "scheme : " << scheme << std::endl;
        autorityEnd = uri.find("/", schemeEnd + 2);
        autority = uri.substr(schemeEnd, autorityEnd);
        // std::cout << "autority : " << autority << std::endl;
        requestedPath = uri.substr(autorityEnd, uri.find("?", autorityEnd));
        // std::cout << "path requested : " << requestedPath  << std::endl;
        queryParameters = uri.substr(uri.find("?", autorityEnd));
        // std::cout << "queryParameters : " << queryParameters << std::endl;
    }
    requestedPath = uri.substr(uri.find("/"));
    // std::cout << "if the uri relative : " <<  requestedPath << std::endl;
    // std::cout << "=====> the end of parseURI" << std::endl;
}

void    Request::parseRequestLine(std::string    &buff)
{
    std::vector<std::string>    splitReqLine;
    std::string                 reqLine;
    
    // std::cout << ">>>>>> here" << std::endl;
    // std::cout << "=====> parse request line : " << std::endl;
    reqLine = buff.substr(0 ,buff.find("\r\n"));
    // std::cout << "-------> heeeere " << std::endl;
    // std::cout << "req line : "<< reqLine << std::endl;
    buff = buff.substr(buff.find("\r\n") + 2);
    // std::cout << "the rest in buff : " << buff << std::endl;
    splitReqLine = StringOperations::split(reqLine, " ");
    // if (splitReqLine.size() != 3)//to add
    method = splitReqLine[0];
    // std::cout << "method : " << method << std::endl;
    uri =splitReqLine[1];
    // std::cout << "uri : " << uri << std::endl;
    protocolVersion = splitReqLine[2];
    // std::cout << "protocol version : " << protocolVersion << std::endl;
    // std::cout << "=========> the end of parse request line : "  << buff << std::endl;
}

void    Request::parseHeader(std::string &buff)
{
    std::string key, header, value;

    // std::cout << "====> parse header : " << std::endl;
    // std::cout << "-----------> " << buff << std::endl;
    header = buff.substr(0, buff.find("\r\n"));
    // std::cout << "----> find \r\n  : " << buff.find("\r\n") << std::endl;
    // std::cout << "========> header : " << header << std::endl;
    buff = buff.substr(buff.find("\r\n") + 2);
    // std::cout << "---------> buff : " << buff << std::endl;
    key = header.substr(0, header.find_first_of(":"));
    for (size_t i = 0; i < key.length(); i++)
        key[i] =  tolower(key[i]);
    // std::for_each(key.begin(), key.end(), ::tolower);
    // std::cout << "header key : " << key << std::endl;
    value = StringOperations::trim(header.substr(key.length() + 1));
    headers[key] = value;
    // std::cout << "======> the end of parse header : " << std::endl;
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
    if (prefix[i] == '/')
        return i;
    return 0;
}

void    Request::matchingLocation()
{
    Server  matchServer;
    size_t  longestOne, sizeMatching;

    matchServer = findServer();
    server = matchServer.getServerData();
    longestOne = 0;
    for (std::map<std::string, mapStrVect>::const_iterator i = matchServer.getLocations().begin();
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
