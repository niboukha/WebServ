/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:32:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/22 10:04:38 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post( Response &response ) : res( response ), size(0), dirflag(0)
{

}

Post::~Post( )
{

}

std::string	Post::conctRootUpload(std::string s)
{
	mapStrVect	loc;
	std::string	pt;

	loc = res.getRequest().getLocation();
	pt = loc["root"].front() + s + "/";
	return (pt);
}

bool	Post::isUploadPass()
{
	mapStrVect	loc;
	std::string	s;

	loc = res.getRequest().getLocation();

	if (loc["upload-pass"].empty())
	{
		s = res.concatenatePath();
		res.setPath(s);
		return (false);
	}

	s  = conctRootUpload(loc["upload-pass"].front());
	s += Utils::generateRundFile();
	res.setPath(s);
	return (true);
}

size_t	Post::maxBodySize()
{
	std::map<std::string, std::string>	ser;
	std::stringstream	s;
	size_t				n;	

	ser  = res.getRequest().getServer();
	s << ser["client_max_body_size"];
	s >> n;
	return (n);
}

void	Post::nonChunkedTransfer()
{
	std::map<std::string, std::string>	ser;

	std::map<std::string, std::string>			head;
	std::stringstream	s;
	std::ofstream		outfile;
	size_t				n;	

	ser  = res.getRequest().getServer();
	head = res.getRequest().getHeaders(); 
	s << head["content-length"];
	s >> n;
	if (maxBodySize() > n)
		res.throwNewPath("413 Request Entity Too Large", "413");
	outfile.open(res.getPath().c_str(), std::ios_base::app);
	size += res.getBody().length();
	outfile << res.getBody();
	if (size == n)
	{
		outfile.close();
		res.throwNewPath("201 Created", "201");
	}
}

int	Post::getLength(std::string s)
{
	std::stringstream	st;
	int					n;

	st << s;
	st >> std::hex >> n;
	return (n);
}

void	Post::chunkedTransfer(std::string body)
{
	std::ofstream	outfile;
	std::string		lrn;
	size_t			foundLen;
	size_t			foundBuff;
	int				len;
	
	outfile.open(res.getPath().c_str(), std::ios_base::app);

	while (body.length())
	{
		foundLen = body.find("\r\n");
		if (foundLen != std::string::npos)
			lrn = body.substr(0, foundLen);
		else
			break;

		len = getLength(lrn);
		size += len;
		if (len == 0)
		{
			outfile.close();
			res.throwNewPath("201 Created", "201");
		}
		if (maxBodySize() > size)
			res.throwNewPath("413 Request Entity Too Large", "413");
		if (lrn.empty())
			break;
		foundBuff = body.find("\r\n", len);
		if (foundBuff != std::string::npos)
			outfile << body.substr(foundLen + 2, len);
		else
			break;
		body = body.substr(foundBuff + 2);
	}
	res.setBody(body);
}

void	Post::cgiPassCheck()
{
	mapStrVect	loc;

	loc = res.getRequest().getLocation();
	if (loc["cgi-pass"].front().empty())
		res.throwNewPath("403 forbidden", "403");
	//else exist
}

void	Post::directoryInRequest(std::string &file)
{
	mapStrVect	loc;

	loc = res.getRequest().getLocation();

	if (file[file.length() - 1] != '/')
	{
		dirflag = 1;
		res.setPath(res.getPath() + "/");
		res.throwNewPath("301 Moved Permanently", "301");
	}

	if (loc["index"].empty())
		res.throwNewPath("403 forbidden", "403");
	cgiPassCheck();
}

void	Post::unsupportedUpload()
{
	std::string		s;
	
	s = res.getPath();
	std::ifstream	file(s.c_str());

	if (!Utils::isDir(s.c_str()))
		directoryInRequest(s);
	if (!file.is_open())
		res.throwNewPath("404 not found", "404");
	else
		cgiPassCheck();
	file.close();
}

void	Post::requestedStatus(int stage)
{
	std::map<std::string, std::string>	header;

	header = res.getRequest().getHeaders();
	if (!isUploadPass() && stage == REQBODY)
	{
		if (header.find("content-length") != header.end())
			nonChunkedTransfer();
		chunkedTransfer( res.getBody() );
	}
	else
	{
		
		unsupportedUpload();
	}
}

// std::string	Post::httpRedirection()
// {
// 	mapStrVect	loc;

// 	loc = res.getRequest().getLocation();
// 	if (loc.find("301") != loc.end())
// 		return (loc["301"].front());
// 	else
// 		return (loc["302"].front());
// }

std::string	Post::responsHeader(int stage)
{
	std::string	s;
	std::string	pt;

	requestedStatus(stage);
	pt = res.getPath();
	s  = res.getRequest().getProtocolVersion() + " " +
		res.getStatusCodeMsg() + CRLF +
		"Content-Type: "   + res.getContentType(pt)   + CRLF +
		"Content-Length: " + res.getContentLength(pt);
	if (dirflag == 1)
		s = s + CRLF + "Location: " + res.getPath();
	s = s + CRLF + CRLF;
	return (s);
}

std::string	Post::responsBody()
{
	char buffer[1024];

	fd = open(res.getPath().c_str(), O_RDWR); //can hang ??

	read(fd, buffer, sizeof(buffer));
	return (buffer);
}

