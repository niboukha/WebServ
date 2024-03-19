/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:16:09 by shicham           #+#    #+#             */
/*   Updated: 2024/03/18 00:34:27 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Request/Request.hpp"

Request::Request(std::vector<Server>& servers) : servs(servers)
{
    fillErrorPages();
    fillLocation();
}

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

const std::string   Request::getQueryParameters( ) const
{
    return (queryParameters);
}

const std::string   Request::getClientIp( ) const
{
    return (clientIp);
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

void    Request::setMethod(std::string method)
{
    this->method = method;
}

void    Request::setProtocolVersion(std::string httpVersion)
{
    protocolVersion = httpVersion;
}


void    Request::fillLocation()
{
    std::vector<std::string> vect;

    vect.push_back("");
    location["root"] = vect;
    location["allow_methodes"] = vect;
    location["autoindex"] = vect;
    location["index"] = vect;
    location["cgi_pass"] = vect;
    location["return"] = vect;
}

void    Request::fillErrorPages()
{
    errorPages["201"] = ERROR_201;
    errorPages["204"] = ERROR_204;
    errorPages["301"] = ERROR_301;
    errorPages["401"] = ERROR_401;
    errorPages["403"] = ERROR_403;
    errorPages["404"] = ERROR_404;
    errorPages["409"] = ERROR_409;
    errorPages["413"] = ERROR_413;
    errorPages["500"] = ERROR_500;
    errorPages["501"] = ERROR_501;
    errorPages["400"] = ERROR_400;
    errorPages["414"] = ERROR_414;
    errorPages["405"] = ERROR_405;
    errorPages["408"] = ERROR_408;
    errorPages["505"] = ERROR_505;
    errorPages["411"] = ERROR_411;
    errorPages["504"] = ERROR_504;
    errorPages["502"] = ERROR_502;
}

void    Request::parseRequestLine(std::string    &buff)
{
    std::vector<std::string>    splitReqLine;
    std::string                 reqLine;
    size_t                      found;
    std::string                 methodes[9] = {"GET", "POST", "DELETE", 
                                                "PUT", "CONNECT", "OPTIONS", "TRACE", 
                                                "HEAD", "PATCH"}; 
    std::string                 methodesImp[3] = {"GET", "POST", "DELETE"};
  
    found = buff.find("\r\n");
    reqLine = buff.substr(0 ,found);
    if (std::count(reqLine.begin(), reqLine.end(), ' ') != 2)
        throw std::make_pair("400", "400 Bad Request");
    buff = buff.substr(found + 2);
    splitReqLine = StringOperations::split(reqLine, " ");
    if ( splitReqLine.size() != 3 
        or (std::find(methodes, methodes + 9, splitReqLine[0]) \
        == (methodes +  9)))
        throw std::make_pair("400", "400 Bad Request");
    if (std::find(methodesImp, methodesImp + 3 , splitReqLine[0]) == (methodesImp +  3))
        throw std::make_pair("501", "501 Not Implemented");
    if (splitReqLine[1].find("/"))
        throw std::make_pair("400", "400 Bad Request");
    if (splitReqLine[2].compare(0, 5,"HTTP/"))
        throw std::make_pair("400", "400 Bad Request");
    if (!splitReqLine[2].compare(0, 5,"HTTP/")  and splitReqLine[2].compare(5, 3, "1.1"))
        throw std::make_pair("505", "505 HTTP Version Not Supported");
    uri = splitReqLine[1];
    method = splitReqLine[0];
    protocolVersion = splitReqLine[2];
}

void    Request::parseHeader(std::string &buff, size_t& found)
{
    std::string key, header, value;
    size_t  pos;
    
   
    header = buff.substr(0, found);
    buff = buff.substr(found + 2);
    pos = header.find_first_of(":");
    if (pos != std::string::npos)
   { 
        key = header.substr(0, header.find_first_of(":"));
        if (key.find_first_of(" \t") != std::string::npos )
            throw std::make_pair("400", "400 Bad Request");
        std::transform(key.begin(), key.end(), key.begin(), tolower);
        value = StringOperations::trim(header.substr(key.length() + 1));
        if (!key.compare("host") and value.empty())
            throw std::make_pair("400", "400 Bad Request");
        if ((!key.compare("host") or !key.compare("content-length") 
            or !key.compare("transfer-encoding")) and headers.find(key) != headers.end())
               throw std::make_pair("400", "400 Bad Request");
        if (!key.compare("content-type") and headers.find(key) != headers.end())
            headers[key] = headers[key] + ";" + value;
        else
            headers[key] = value;
    }   
}

void    Request::validateRequestHeader()
{
    if (headers.find("host") ==  headers.end())
        throw std::make_pair("400", "400 Bad Request");
        
    std::map<std::string, std::string>::iterator transferEncIt = headers.find("transfer-encoding"), 
                                                contentLenIt = headers.find("content-length"),
                                                contentTypeIt = headers.find("content-type");
    int                                         isMethodPost;
                                      
    if (contentLenIt != headers.end())
    {
        (contentLenIt->second.empty()) ? throw std::make_pair("400", "400 Bad Request") : false;
        
        std::istringstream  iss(headers["content-length"]);
        long long           contentLen;
        std::string            strLongLong;
        char                remain;
        std::ostringstream oss;
    
        if (!(iss >> contentLen) or (iss >> remain) or (contentLen < 0))
           throw std::make_pair("400", "400 Bad Request");
        oss << contentLen;
        strLongLong = oss.str();
        if (strLongLong.compare(headers["content-length"]))
            throw std::make_pair("400", "400 Bad Request");
    }
    if (transferEncIt != headers.end())
    {
        (transferEncIt->second.empty()) ? throw std::make_pair("400", "400 Bad Request") : false;
        (transferEncIt->second.compare("chunked")) ?  throw std::make_pair("501", "501 Not Implemented") : false;
    }
    if (contentTypeIt != headers.end() )
        (contentTypeIt->second.empty()) ? throw std::make_pair("400", "400 Bad Request") : false;
    isMethodPost = method.compare("POST");
    
    if (!isMethodPost and transferEncIt == headers.end()
    and contentLenIt == headers.end())
        throw std::make_pair("411", "411 Length Required");
        
    if (!isMethodPost and contentLenIt != headers.end()
        and transferEncIt != headers.end())
        throw std::make_pair("400", "400 Bad Request");

    // if we found an error in content type
    
} 

void   Request::parseRequest(std::string &buff, Stage &stage)
{
    if (stage == REQLINE)
    {
        parseRequestLine(buff);
        parseUri();
        stage = REQHEADER;
    }
    else if (stage == REQHEADER)
    {
        size_t  pos = buff.find("\r\n");
        
        if (!pos)
        {
            validateRequestHeader();
            matchingLocation();
            buff = buff.substr(pos + 2);
            stage = REQBODY;
            return ;
        }
        parseHeader(buff, pos);
    }
}

void    Request::decodeUri()
{
    char    decodeChar;

    for (size_t i = 0; i < requestedPath.size(); i++)
    {
        if((requestedPath[i] == '%' and i != requestedPath.length() - 2) and 
        ((isdigit(requestedPath[i + 1]) and isupper(requestedPath[i + 2]))
        or (isdigit(requestedPath[i + 1]) and isdigit(requestedPath[i + 2]))))
        {
            decodeChar = static_cast<char>(strtol(requestedPath.substr(i + 1, 2).c_str(), NULL, 16));
            requestedPath.replace(i, 3, 1, decodeChar);
            i += 3;
        }
    }
}

void   Request::parseUri()
{
    size_t      queryDelim;

    if (uri.length() > 2048)
        throw std::make_pair("414", "414 URI Too Long");
    if (uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijkl\
        mnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
        throw std::make_pair("400", "400 Bad Request");
    queryDelim = uri.find("?");
    if (queryDelim != std::string::npos)
    {
        queryParameters = uri.substr(queryDelim + 1);
        requestedPath = uri.substr(uri.find("/"), queryDelim);
        return ;
    }
    requestedPath = uri.substr(uri.find("/"));
    decodeUri();
}


size_t  Request::longestMatchingLocation(const std::string& prefix)
{
    size_t i = 0;
    for (; i < uri.size() and uri[i] == prefix[i]; i++)
    {}
    if (uri[i - 1] == '/') 
        return i;
    return 0;
}

void    Request::matchingLocation()
{
    Server& matchingServ = matchingServer();
    size_t  longestOne, sizeMatching;
    std::string subUri;
    std::vector<std::string>::iterator it;

    clientIp = matchingServ.getClientIp();
    server   = matchingServ.getServerData();
    longestOne = 0;

    for (std::map<std::string, mapStrVect>::const_iterator 
        i = matchingServ.getLocations().begin();
        i != matchingServ.getLocations().end(); i++)
    {
        sizeMatching = longestMatchingLocation((i->first));
        if (longestOne < sizeMatching)
        {
            subUri = i->first;
            location = i->second;  
            longestOne = sizeMatching;
        }
    }

    
    if (subUri.empty())
        throw std::make_pair("404", "404 Not found");
    
    if (requestedPath.find("/", subUri.length() - 1) != std::string::npos)
        requestedPath = requestedPath.substr(requestedPath.find("/", subUri.length() - 1));
        
    if (!location["return"].front().empty())
    {
        throw std::make_pair(((location["return"][0]).c_str()),
            ("301 Moved Permanently"));
    }
    if (!location["allow_methodes"].front().empty())
    {
        it = find(location["allow_methodes"].begin(), 
        location["allow_methodes"].end(), method);
        it == location["allow_methodes"].end() ? throw std::make_pair("405", "405 Method Not Allowed") : false;
    }
}

Server&  Request::matchingServer()
{
    std::string host, port;
    std::string serverName;
    size_t      found;

    found = headers.find("host")->second.find(":");
    if (found != std::string::npos)
    {
        host = headers.find("host")->second.substr(0, found);
        port =  headers.find("host")->second.substr(found + 1);
    }
    else 
        host = headers.find("host")->second;
    
    for (size_t i = 0; i < servs.size(); i++)
    {
        
        servs[i].getServerData().find("server_name") != servs[i].getServerData().end() ? 
        serverName = servs[i].getServerData().find("server_name")->second : serverName = "";
        if (!serverName.compare(host) and !port.empty() and  
        !servs[i].getServerData().find("port")->second.compare(port))
            return servs[i];
    }
    return servs[0];
}