/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:16:09 by shicham           #+#    #+#             */
/*   Updated: 2024/03/09 18:48:35 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Request/Request.hpp"

Request::Request(std::vector<Server>& servers) : servs(servers)
{
    fillErrorPages();
}

Request::Request()
{   
}

Request::~Request()
{
    // std::cout << "=========> here ======" << std::endl;
}

// const Request&  Request::operator=(const Request& copy)// to check mn b3ed !!!
// {
//     if (this != &copy)
//     {
//     	configFileData  = copy.configFileData;
// 	    location        = copy.location;
// 		server          = copy.server;
// 		headers         = copy.headers;
// 		errorPages      = copy.errorPages;
// 		method          = copy.method;
// 		requestedPath   = copy.requestedPath;
// 		protocolVersion = copy.protocolVersion;
// 		uri             = copy.uri;
// 		autority        = copy.autority;
// 		scheme          = copy.scheme;
// 		queryParameters = copy.queryParameters;
//     }
//     return *this;
// }
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

void    Request::setMethod(std::string method)
{
    this->method = method;
}

void    Request::setProtocolVersion(std::string httpVersion)
{
    protocolVersion = httpVersion;
}

void    Request::validateRequestHeader()
{
    long long   contentLen;
    char        *end;
    
    if (headers.find("host") ==  headers.end())
        throw std::make_pair("400", "400 Bad Request");
        
    std::map<std::string, std::string>::iterator transferEncIt = headers.find("transfer-encoding"), 
                                                contentLenIt = headers.find("content-length"),
                                                contentTypeIt = headers.find("content-type");
    int                                         isMethodPost;
                                                
    if (contentLenIt != headers.end())
    {
        (contentLenIt->second.empty()) ? throw std::make_pair("400", "400 Bad Request") : false;
        contentLen = strtoll(headers["content-length"].c_str(), &end, 10);
        (contentLen < 0 or *end) ?  throw std::make_pair("400", "400 Bad Request") : false;
    }
    if (transferEncIt != headers.end())
    {
        (transferEncIt->second.empty()) ? throw std::make_pair("400", "400 Bad Request") : false;
        (transferEncIt->second.compare("chunked")) ?  throw std::make_pair("501", "501 Not Implemented") : false;
    }
    if (contentTypeIt != headers.end() )
    {
        std::cout << "here content type " << std::endl;
        (contentTypeIt->second.empty()) ? throw std::make_pair("400", "400 Bad Request") : false;
        (contentTypeIt->second.find("boundary=") != std::string::npos) ? throw std::make_pair("501", "501 Not Implemented") : false;
    }
    isMethodPost = method.compare("POST");
    if ((!isMethodPost and transferEncIt == headers.end() 
        and contentLenIt == headers.end())
        or (!isMethodPost and contentLenIt != headers.end() 
        and transferEncIt != headers.end()))//bad req
        throw std::make_pair("400", "400 Bad Request");
    if (!isMethodPost and
        (contentTypeIt == headers.end()))
       throw std::make_pair("400", "400 Bad Request");
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
            // std::cout << "THE END OR PARSING" << std::endl;
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

    for (size_t i = 0; i < uri.size(); i++)
    {
        if((uri[i] == '%' and i != uri.length() - 2) and 
        ((isdigit(uri[i + 1]) and isupper(uri[i + 2]))
        or (isdigit(uri[i + 1]) and isdigit(uri[i + 2]))))
        {
            decodeChar = static_cast<char>(strtol(uri.substr(i + 1, 2).c_str(), NULL, 16));
            uri.replace(i, 3, 1, decodeChar);
            i += 3;
        }
    }
}

void   Request::parseUri()//to check 
{
    size_t      queryDelim;

    decodeUri();
    if (uri.length() > 2048)//bad req
        throw std::make_pair("414", "414 Request-URI too long");//to large 
    if (uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijkl\
        mnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
        throw std::make_pair("400", "400 Bad Request");
    queryDelim = uri.find("?");
    if (queryDelim != std::string::npos)
    {
        queryParameters = uri.substr(queryDelim);
        requestedPath = uri.substr(uri.find("/"), queryDelim);
        return ;
    }
    requestedPath = uri.substr(uri.find("/"));
}

void    Request::parseRequestLine(std::string    &buff)
{
    std::vector<std::string>    splitReqLine;
    std::string                 reqLine;
    size_t                      found;
    std::string                 methodes[9] = {"GET", "POST", "DELETE", 
                                                "PUT", "CONNECT", "OPTIONS", "TRACE", 
                                                "HEAD", "PATCH"}; //PATCH//HEAD FIHA BLAN
    std::string                 methodesImp[3] = {"GET", "POST", "DELETE"};
  
    found = buff.find("\r\n");
    reqLine = buff.substr(0 ,found);
    if (std::count(reqLine.begin(), reqLine.end(), ' ') != 2)//Bad req to check \r\n
        throw std::make_pair("400", "400 Bad Request");
    buff = buff.substr(found + 2);
    splitReqLine = StringOperations::split(reqLine, " ");
    if ( splitReqLine.size() != 3 
        or (std::find(methodes, methodes + 9, splitReqLine[0]) \
        == (methodes +  9)))//Bad request can cz SGV !!!
        throw std::make_pair("400", "400 Bad Request");
    if (std::find(methodesImp, methodesImp + 3 , splitReqLine[0]) == (methodesImp +  3))//Not implemented
        throw std::make_pair("501", "501 Not Implemented");
    if (splitReqLine[1].find("/"))
        throw std::make_pair("400", "400 Bad Request");
    if (splitReqLine[2].compare(0, 5,"HTTP/"))
        throw std::make_pair("400", "400 Bad Request");
    if (!splitReqLine[2].compare(0, 5,"HTTP/")  and splitReqLine[2].compare(5, 3, "1.1"))
        throw std::make_pair("505", "505 not supported");
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
        std::transform(key.begin(), key.end(), key.begin(), tolower);//to check if it exist in cpp98
        value = StringOperations::trim(header.substr(key.length() + 1));//to check trim !!!!!
        if (!key.compare("host") and value.empty())
            throw std::make_pair("400", "400 Bad Request");
        if ((!key.compare("host") or !key.compare("content-length") 
            or !key.compare("transfer-encoding")) and headers.find(key) != headers.end())
               throw std::make_pair("400", "400 Bad Request");
        if (!key.compare("content-type") and headers.find(key) != headers.end())
            headers[key] = headers[key] + ";" + value;
        else
            headers[key] = value;//check if key exist
    }   
}

size_t  Request::longestMatchingLocation(const std::string& prefix)
{
    size_t i = 0;
    for (; i < uri.size() and uri[i] == prefix[i]; i++)
    {}
    if (uri[i - 1] == '/') //should check after
        return i;
    return 0;
}

void    Request::matchingLocation()
{
    Server& matchingServ = matchingServer();
    size_t  longestOne, sizeMatching;
    std::string subUri;
    
    server = matchingServ.getServerData();
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
    if (location["return"].empty())//to check mn b3ed !!!
        throw std::make_pair((location["return"][0]).c_str(),\
        (location["return"][0] + "Move Permantly").c_str());
    requestedPath = requestedPath.substr(0, subUri.find_last_not_of(requestedPath));
}

Server&  Request::matchingServer()
{
    std::string serverName;

    serverName = headers.find("host")->second;
    for (size_t i = 0; i < servs.size(); i++)
    {
        if (!servs[i].getServerData().find("server_name")->second.compare("host"))
            return servs[i];
    }
    return servs[0];
}
