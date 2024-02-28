/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:16:09 by shicham           #+#    #+#             */
/*   Updated: 2024/02/28 07:06:01 by niboukha         ###   ########.fr       */
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

const	std::map<std::string, std::string>	Request::getErrorPages() const
{
    return (errorPages);
}

void    Request::fillErrorPages()
{
    errorPages["201"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/201.html";
    errorPages["204"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/204.html";
    errorPages["301"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/301.html";
    errorPages["401"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/401.html";
    errorPages["403"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/403.html";
    errorPages["404"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/404.html";
    errorPages["409"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/409.html";
    errorPages["413"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/413.html";
    errorPages["500"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/500.html";
    
    errorPages["501"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/501.html";
    errorPages["400"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/400.html";
    errorPages["414"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/414.html";
    errorPages["405"] = "/nfs/homes/niboukha/Desktop/webserv/ErrorPages/405.html";
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
        if (!buff.find("\r\n"))
        {
            if (headers.find("transfer-encoding") != headers.end()
                and headers["transfer-encoding"].compare("chunked"))//not implemented to check mn b3d
               throw Request::NotImplemented("501", "/ErrorPages/400.html");
            if (headers.find("transfer-encoding") == headers.end() 
                and headers.find("content-length") == headers.end() 
                and !method.compare("POST"))//bad req
              throw Request::BadRequest("400", "/ErrorPages/400.html");
            // if (headers.find("host") == headers.end())//to check mn b3ed
            matchingLocation();
            fillErrorPages();
            buff = buff.substr(buff.find("\r\n") + 2);
            stage = REQBODY;
            return ;
        }
        parseHeader(buff);
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
    
    //check if the uri start with /
    decodeUri();
    if (uri.length() > 2048)//bad req
        throw Request::BadRequest("400", "/ErrorPages/400.html");
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
    else
        requestedPath = uri.substr(uri.find("/"));
}

void    Request::parseRequestLine(std::string    &buff)
{
    std::vector<std::string>    splitReqLine;
    std::string                 reqLine;
    size_t                      found;
    std::string                 methodes[8] = {"GET", "POST", "DELETE", 
                                            "PUT", "CONNECT", "OPTIONS", "TRACE", "HEAD"};
    std::string                 methodesImp[3] = {"GET", "POST", "DELETE"};
  
    found = buff.find("\r\n");
    if (found == std::string::npos)//bad req to check mn b3d
        throw Request::BadRequest("400", "/ErrorPages/400.html");
    reqLine = buff.substr(0 ,found);
    if (found != (reqLine.length()) or 
        std::count(reqLine.begin(), reqLine.end(), ' ') != 2)//Bad req to check \r\n
       throw Request::BadRequest("400", "/ErrorPages/400.html");
    buff = buff.substr(found + 2);
    splitReqLine = StringOperations::split(reqLine, " ");
    if ( splitReqLine.size() != 3 
        or (std::find(methodes, methodes + 8, splitReqLine[0]) == (methodes +  8))
        or splitReqLine[2].compare("HTTP/1.1"))//Bad request can cz SGV !!!
        throw Request::BadRequest("400", "/ErrorPages/400.html");
    method = splitReqLine[0];
    if (std::find(methodesImp, methodesImp + 3, method) \
        == (methodes +  3))//Not implemented
       throw Request::NotImplemented("501", "/ErrorPages/400.html");
    uri = splitReqLine[1];
    protocolVersion = splitReqLine[2];
}

void    Request::parseHeader(std::string &buff)
{
    std::string key, header, value;
    size_t      found;

    found = buff.find("\r\n");
    if (found == std::string::npos)//bad req to check 
        throw Request::BadRequest("400", "/ErrorPages/400.html");
    header = buff.substr(0, found);
    if (found != header.length())//bad req
        throw Request::BadRequest("400", "/ErrorPages/400.html");
    buff = buff.substr(found + 2);
    if (!header.empty())//update
    { 
        key = header.substr(0, header.find_first_of(":"));
        key = StringOperations::trim(key);
        std::transform(key.begin(), key.end(), key.begin(), tolower);//to check if it exist in cpp98
        value = StringOperations::trim(header.substr(key.length() + 1));
        headers[key] = value;
    }
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

    matchServer = matchingServer();
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

Server  Request::matchingServer()
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
