/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:32:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/27 16:58:53 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Post.hpp"

// long long	Post::uploadSize = 0;

Post::Post( Response &response ) :	res( response ),
									size( 0 ),
									isMoved( false ),
									sizeofRead( 0 ),
									enter( false ),
									uploadSize( 0 ),
									saveOffset( 0 )
{
}

Post::~Post( )
{

}

const std::streampos	Post::getSizeofRead() const
{
	return (sizeofRead);
}

std::string	Post::conctRootUpload(std::string s)
{
	mapStrVect	loc;
	std::string	pt;

	loc = res.getRequest().getLocation();
	pt  = loc["root"].front() + s + "/";
	return (pt);
}

long long	Post::maxBodySize( )
{
	std::map<std::string, std::string>	ser;
	std::stringstream					s;
	long long							n;

	ser  = res.getRequest().getServer();
	s << ser["client_max_body_size"];
	s >> n;
	return ( n );
}

bool	Post::isUploadPass( Stage &stage )
{
	mapStrVect	loc;
	std::string	s;
	struct stat	statPath;
	
	if (enter == true)
		return (enter);

	enter = true;
	loc   = res.getRequest().getLocation();
	if (loc["upload_pass"].empty())
	{
		stage = RESHEADER;
		s     = res.concatenatePath( res.getRequest().getRequestedPath() );
		res.setPath(s);
		return (false);
	}
	if (!Utils::isDir(conctRootUpload(loc["upload_pass"].front()).c_str()))
		res.throwNewPath("404 not found", "404");
	if (!stat(conctRootUpload(loc["upload_pass"].front()).c_str(), &statPath)
		and !(statPath.st_mode & S_IWUSR))
		res.throwNewPath("403 Forbidden", "403");
	
	s = conctRootUpload(loc["upload_pass"].front())
		+ Utils::generateRundFile() + "." + res.getExtensionFile();
	res.setPath( s );
	return ( true );
}

void	Post::nonChunkedTransfer(Stage &stage)
{
	std::map<std::string, std::string>	head;
	std::ofstream						outfile;

	head = res.getRequest().getHeaders(); 

	if (maxBodySize() < Utils::stringToLong(head["content-length"]))
	{
		stage = RESHEADER;
		res.throwNewPath("413 Request Entity Too Large", "413");
	}
	if (!outfile.is_open())
		outfile.open(res.getPath().c_str(), std::ios_base::app);

	outfile << res.getBody();
	std::cout << uploadSize << " " << Utils::stringToLong(head["content-length"]) << "\n";
	uploadSize += res.getBody().size();
	if (uploadSize == Utils::stringToLong(head["content-length"]))
	{
		stage = RESHEADER;
		outfile.close();
		res.throwNewPath("201 Created", "201");
	}
}

void	Post::chunkedTransfer(std::string body, Stage &stage)
{
	std::ofstream	outfile;
	std::string		lrn;
	size_t			foundLen;
	size_t			foundBuff;
	int				hexLen;

	if (!outfile.is_open())
		outfile.open(res.getPath().c_str(), std::ios_base::app);

	while (body.size())
	{
		foundLen = body.find("\r\n");
		if (foundLen != std::string::npos)
		{
			lrn = body.substr(0, foundLen);
		}
		else
			break;

		hexLen = Utils::getLength(lrn);
		if (hexLen == 0)
		{
			stage = RESHEADER;
			outfile.close();
			res.throwNewPath("201 Created", "201");
		}
		if (lrn.empty())
			break;
		foundBuff = body.find("\r\n", hexLen);
		if (foundBuff != std::string::npos)
		{
			size += hexLen;
			if (maxBodySize() < size)
				res.throwNewPath("413 Request Entity Too Large", "413");
			outfile << body.substr(foundLen + 2, hexLen);
			body = body.substr(foundBuff + 2);
		}
		else
			break;
	}
}

void	Post::cgiPassCheck()
{
	mapStrVect	loc;

	loc = res.getRequest().getLocation();
	if (loc["cgi_pass"].front().empty())
		res.throwNewPath("403 forbidden", "403");
	//else exist
}

void	Post::directoryInRequest(std::string &path, std::ifstream &file)
{
	mapStrVect	loc;

	loc = res.getRequest().getLocation();
	if (path[path.length() - 1] != '/')
	{
		isMoved = true;
		res.setPath(res.getPath() + "/");
		file.close();
		res.throwNewPath("301 Moved Permanently", "301");
	}
	if (loc["index"].empty())
	{
		file.close();
		res.throwNewPath("403 forbidden", "403");
	}
	// else
		//cgi
}

void	Post::unsupportedUpload( )
{
	std::ifstream	file(res.getPath().c_str());
	std::string		s;

	s = res.getPath();
	if (Utils::isDir(s.c_str()))
		directoryInRequest(s, file);
	if (!Utils::isFile(s.c_str()))
	{
		file.close();
		res.throwNewPath("404 not found", "404");
	}
	// else
		//cgi
	file.close();
}

void	Post::requestedStatus(Stage &stage)
{
	std::map<std::string, std::string>	header;
	
	header = res.getRequest().getHeaders();
	if (isUploadPass( stage ) == true and stage == REQBODY)
	{
		if (header.find("content-length") != header.end())
			nonChunkedTransfer(stage);
		else
			chunkedTransfer( res.getBody(), stage );
	}
	else if (stage > REQBODY)
		unsupportedUpload();
}

std::string	Post::responsHeader(Stage &stage)
{
	std::string	s;
	std::string	pt;

	if (res.getStatusCodeMsg() != "-1")
		res.setPath(res.concatenatePath(res.getPath()));
	requestedStatus(stage);
	pt = res.getPath();
	s  = res.getRequest().getProtocolVersion() + " " +
		res.getStatusCodeMsg() + CRLF +
		"Content-Type: "       + res.getContentType(pt) + CRLF +
		"Content-Length: "     + res.getContentLength(pt);

	if (isMoved == true)
		s = s + CRLF + "Location: " + res.getPath();
	s = s + CRLF + CRLF;
	return (s);
}

std::string	Post::responsBody()
{
	std::ifstream	in(res.getPath().c_str(), std::ios_base::binary);
	char			buff[20];

	in.seekg(saveOffset, std::ios::cur);
	in.read(buff, sizeof(buff));
	sizeofRead  = in.gcount();
	saveOffset += sizeofRead;

	if (sizeofRead == 0)
		in.close();
	return (std::string(buff, sizeofRead));
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
