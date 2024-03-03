/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:32:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/01 19:08:54 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Post.hpp"

Post::Post( Response &response ) :	res( response ),
									size( 0 ),
									isMoved( false ),
									sizeofRead( 0 ),
									enter( false ),
									uploadSize( 0 ),
									saveOffset( 0 ),
									contentLengthLong ( 0 )
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
	const mapStrVect	&loc = res.getRequest().getLocation();
	std::string			pt;

	if (loc.find("root") != loc.end())
		pt  = loc.find("root")->second.front() + s + "/";
	return (pt);
}

long long	Post::maxBodySize( )
{
	const std::map<std::string, std::string>	&ser = res.getRequest().getServer();
	std::stringstream							s;
	long long									n;

	s << ser.find("client_max_body_size")->second;
	s >> n;
	return ( n );
}

void	Post::nonChunkedTransfer(Stage &stage, std::string &reqBuff)
{
	const std::map<std::string, std::string>	&head = res.getRequest().getHeaders();

	if (!UploadFile.is_open())
	{
		UploadFile.open(res.getPath().c_str(), std::ios_base::app);
		contentLengthLong = Utils::stringToLong(head.find("content-length")->second);
		if (maxBodySize() < contentLengthLong)
		{
			stage = RESHEADER;
			res.throwNewPath("413 Request Entity Too Large", "413");
		}
	}
	UploadFile << reqBuff;
	uploadSize += reqBuff.size();
	if (uploadSize == contentLengthLong)
	{
		stage = RESHEADER;
		UploadFile.close();
		res.throwNewPath("201 Created", "201");
	}
	reqBuff.clear();
}

void	Post::chunkedTransfer(std::string body, Stage &stage)
{
	std::ofstream	outfile;
	std::string		lrn;
	size_t			foundLen;
	size_t			foundBuff;
	long long		hexLen;

	hexLen = 0;
	if (!outfile.is_open())
		outfile.open(res.getPath().c_str(), std::ios_base::app);
	while (body.size())
	{
		foundLen = body.find("\r\n");
		if (foundLen != std::string::npos)
			lrn = std::string(body, 0, foundLen);
		else
			break;

		hexLen = Utils::getLength(lrn);
		if (hexLen == 0)
		{
			outfile << std::string(body, 0, body.size() - 5);
			stage = RESHEADER;
			outfile.close();
			res.throwNewPath("201 Created", "201");
		}
		foundBuff = body.find("\r\n", hexLen);
		if (foundBuff != std::string::npos)
		{
			size += hexLen;
			if (maxBodySize() < size)
				res.throwNewPath("413 Request Entity Too Large", "413");
			// std::string(body, hexLen);
			outfile << std::string(body, foundLen + 2, hexLen);
			body = std::string(body, foundBuff + 2);
			res.setBody(body);
		}
		else
			break;
	}
}

void	Post::cgiPassCheck()
{
	const mapStrVect	&loc = res.getRequest().getLocation();

	if (loc.find("cgi_pass")->second.front().empty())
		res.throwNewPath("403 forbidden", "403");
	//else exist
}

void	Post::directoryInRequest(std::string &path, std::ifstream &file)
{
	const mapStrVect	&loc = res.getRequest().getLocation();

	if (path[path.length() - 1] != '/')
	{
		isMoved = true;
		res.setPath(res.getPath() + "/");
		file.close();
		res.throwNewPath("301 Moved Permanently", "301");
	}
	if (loc.find("index")->second.empty())
	{
		file.close();
		res.throwNewPath("403 forbidden", "403");
	}
	// else
		//cgi
}

void	Post::unsupportedUpload( )
{
	const mapStrVect	&loc = res.getRequest().getLocation();
	std::ifstream		file(res.getPath().c_str());
	std::string			s;

	s   = res.getPath();
	if (Utils::isDir(s.c_str()))
		directoryInRequest(s, file);
	if (!Utils::isFile(s.c_str()))
	{
		file.close();
		res.throwNewPath("404 not found", "404");
	}
	if (loc.find("cgi_pass")->second.empty())
	{
		file.close();
		res.throwNewPath("403 forbidden", "403");
	}
		//cgi
	file.close();
}

bool	Post::isUploadPass( Stage &stage )
{
	const mapStrVect	&loc = res.getRequest().getLocation();
	struct stat			statPath;
	std::string			s;

	if (enter) return (enter);
	enter = true;
	if (loc.find("upload_pass") == loc.end())
	{
		stage = RESHEADER;
		s     = res.concatenatePath( res.getRequest().getRequestedPath() );
		res.setPath(s);
		return (false);
	}
	if (!Utils::isDir(conctRootUpload(loc.find("upload_pass")->second.front()).c_str()))
		res.throwNewPath("404 not found", "404");
	if (!stat(conctRootUpload(loc.find("upload_pass")->second.front()).c_str(), &statPath)
		and !(statPath.st_mode & S_IWUSR))
		res.throwNewPath("403 Forbidden", "403");
	
	s = conctRootUpload(loc.find("upload_pass")->second.front())
		+ Utils::generateRundFile() + "." + res.getExtensionFile();
	res.setPath( s );
	return ( true );
}

void	Post::requestedStatus(Stage &stage, std::string &reqBuff)
{
	const std::map<std::string, std::string>	&header = res.getRequest().getHeaders();

	if (isUploadPass( stage ) and stage == REQBODY)
	{
		if (header.find("content-length") != header.end())
			nonChunkedTransfer(stage, reqBuff);
		else
			chunkedTransfer( res.getBody(), stage );
	}
	else if (stage > REQBODY)
		unsupportedUpload();
}

std::string	Post::responsHeader(Stage &stage, std::string &reqBuff)
{
	std::string	s;
	std::string	pt;

	if (res.getStatusCodeMsg() == "-1")
		requestedStatus(stage, reqBuff);
	pt = res.getPath();
	s  = res.getRequest().getProtocolVersion() + " " +
		res.getStatusCodeMsg() + CRLF +
		"Content-Type: "       + res.getContentType(pt) + CRLF +
		"Content-Length: "     + res.getContentLength(pt);

	if (isMoved) s = s + CRLF + "Location: " + res.getPath();
	s = s + CRLF + CRLF;
	return (s);
}

std::string	Post::responsBody()
{
	std::ifstream	in(res.getPath().c_str(), std::ios_base::binary);
	char			buff[1024];

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
