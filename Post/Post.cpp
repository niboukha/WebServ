/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:32:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/17 18:03:33 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post( Response &res ) :	response       	  ( res ),
								cgi       		  ( res ),
								size      		  ( 0 ),
								sizeofRead		  ( 0 ),
								enter     		  ( false ),
								uploadSize		  ( 0 ),
								contentLengthLong ( 0 ),
								maxBody   		  ( 0 )
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
	const mapStrVect	&loc = response.getRequest().getLocation();
	std::string			pt;

	pt = loc.find("root")->second.front() + s + "/";
	return (pt);
}

long long	Post::maxBodySize( )
{
	const std::map<std::string, std::string>	&ser = response.getRequest().getServer();
	std::stringstream							s;
	long long									n;

	s << ser.find("client_max_body_size")->second;
	s >> n;
	return ( n );
}

void	Post::nonChunkedTransfer( Stage &stage, std::string &reqBuff )
{
	const std::map<std::string, std::string>	&head = response.getRequest().getHeaders();

	if (!UploadFile.is_open())
	{
		UploadFile.open(response.getPath().c_str(), std::ios_base::app);
		
		contentLengthLong = Utils::stringToLong(head.find("content-length")->second);
		if (maxBodySize() < contentLengthLong)
		{
			stage = RESHEADER;
			response.throwNewPath("413 Request Entity Too Large", "413");
		}
	}
	if (static_cast<long long>(uploadSize + reqBuff.size()) > contentLengthLong)
	{
		UploadFile << std::string (reqBuff, 0, contentLengthLong - uploadSize);
		stage = RESHEADER;
		UploadFile.close();
		response.throwNewPath("201 Created", "201");
	}

	UploadFile << reqBuff;

	uploadSize	   += reqBuff.size();
	if (uploadSize == contentLengthLong)
	{
		stage = RESHEADER;
		UploadFile.close();
		response.throwNewPath("201 Created", "201");
	}
	reqBuff.clear();
}

void	Post::chunkedTransfer(std::string &reqBuff, Stage &stage)
{
	size_t			foundLen;
	size_t			foundBuff;
	long long		hexLen;

	if (!UploadFile.is_open())
	{
		UploadFile.open(response.getPath().c_str(), std::ios_base::app);
		maxBody = maxBodySize();
	}
	while (reqBuff.size())
	{
		foundLen	    = reqBuff.find("\r\n");
		if (foundLen   != std::string::npos)
		{
			hexLen		= Utils::getLength(std::string(reqBuff, 0, foundLen));
			if (hexLen == 0)
			{
				stage	= RESHEADER;
				UploadFile.close();
				response.throwNewPath("201 Created", "201");
			}
		}
		else
			break;
		foundBuff	   = reqBuff.find("\r\n", hexLen + foundLen + 2);
		if (foundBuff != std::string::npos)
		{
			size += hexLen;
			if (maxBody < size)
				response.throwNewPath("413 Request Entity Too Large", "413");
			UploadFile << std::string(reqBuff, foundLen + 2, hexLen);
			reqBuff = std::string(reqBuff, foundBuff + 2);	
		}
		else
			break;
	}
}

void	Post::cgiPassCheckment( std::string &reqBuff, Stage &stage, CgiStage &cgiStage )
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	std::string			path = response.getPath();
	 
	if (((loc.find("cgi_pass")->second.front().empty()
		or (loc.find("cgi_pass")->second.front() != ".py" 
		and loc.find("cgi_pass")->second.front() != ".php"))
		and cgiStage 							 != ERRORCGI))
		{
			cgiStage = ERRORCGI;
			response.throwNewPath("403 forbidden", "403");
		}
	else if (cgiStage == INITCGI)
	{
		if (!response.extentionToCgi(path))
		{
			cgiStage = ERRORCGI;
			response.throwNewPath("403 forbidden", "403");
		}
		if (stage >= RESHEADER)
			cgiStage = WAITCGI;
		cgi.executeCgi(reqBuff, stage, cgiStage );
	}
}

void	Post::pathPermission( CgiStage &cgiStage )
{
	struct stat statPath;

	if (!stat(response.getPath().c_str(), &statPath))
		if (!(statPath.st_mode & S_IWUSR))
		{
			cgiStage = ERRORCGI;
			response.throwNewPath("403 Forbidden", "403");
		}
}

std::string	Post::concatenateIndexDirectory( )
{
	mapStrVect  loc;
	
	loc = response.getRequest().getLocation();
	for (size_t  i = 0; i < loc["index"].size(); i++)
	{
		std::ifstream	myFile(loc["index"][i].c_str());
		if (myFile.is_open())
		{
			myFile.close();
			return (loc["root"].front() + loc["index"][i]);
		}
		myFile.close();
	}
	response.throwNewPath("403 forbidden", "403");
	return (NULL);
}

void	Post::directoryInRequest(std::string &reqBuff, std::ifstream &file, Stage &stage, CgiStage &cgiStage)
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	
	if (loc.find("index")->second.empty())
	{
		cgiStage = ERRORCGI;
		file.close();
		response.throwNewPath("403 forbidden", "403");
	}
	else
	{
		response.setPath(concatenateIndexDirectory());
		cgiPassCheckment(reqBuff, stage, cgiStage );
	}
}

void	Post::unsupportedUpload( std::string &reqBuff, Stage &stage, CgiStage &cgiStage )
{
	std::ifstream	file(response.getPath().c_str());
	std::string		s;

	s = response.getPath();
	if (Utils::isDir(s.c_str()))
	{
		pathPermission(cgiStage);
		directoryInRequest(reqBuff, file, stage, cgiStage);
	}
	else if (!Utils::isFile(s.c_str()))
	{
		cgiStage = ERRORCGI;
		file.close();
		response.throwNewPath("404 not found", "404");
	}
	else
		cgiPassCheckment(reqBuff, stage, cgiStage );
	file.close();
}

bool	Post::isUploadPass( )
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	struct stat			statPath;
	std::string			path;

	if (loc.find("upload_pass")->second.front().empty())
	{
		path = response.concatenatePath( response.getRequest().getRequestedPath() );
		response.setPath(path);
		return (false);
	}

	if (enter) return (enter);

	enter = true;
	if (!Utils::isDir(conctRootUpload(loc.find("upload_pass")->second.front()).c_str()))
		response.throwNewPath("500 Internal Server Error", "500");
	if (!stat(conctRootUpload(loc.find("upload_pass")->second.front()).c_str(), &statPath)
		and !(statPath.st_mode & S_IWUSR))
		response.throwNewPath("403 Forbidden", "403");
	
	path = conctRootUpload(loc.find("upload_pass")->second.front())
		   + Utils::generateRundFile() + "." + response.getExtensionFile();
	response.setPath( path );
	return ( true );
}

void	Post::requestedStatus(Stage &stage, std::string &reqBuff, CgiStage &cgiStage)
{
	const std::map<std::string, std::string>	&header = response.getRequest().getHeaders();

	if (isUploadPass( ) and stage == REQBODY)
	{
		if (header.find("content-length") != header.end())
			nonChunkedTransfer( stage, reqBuff );
		else
			chunkedTransfer( reqBuff, stage );
	}
	else if (stage >= REQBODY)
		unsupportedUpload( reqBuff, stage, cgiStage );
}

void	Post::responsHeader(Stage &stage, std::string &reqBuff, std::string &headerRes, CgiStage &cgiStage)
{
	if (cgiStage == WAITCGI)
		cgi.waitCgi(stage, cgi.getPid(), cgiStage);
	else
	{
		std::string	pt;

		if (response.getStatusCodeMsg() == "-1")
			requestedStatus(stage, reqBuff, cgiStage);
		
		if (cgiStage == EXECUTECGI)
		{
			stage 	  = RESBODY;
			headerRes = response.getRequest().getProtocolVersion() + " "  +
				response.getStatusCodeMsg()                         + CRLF;
			return ;
		}
		else if (cgiStage != WAITCGI)
		{
			pt        = response.getPath();
			headerRes = response.getRequest().getProtocolVersion()           +  " " +
						response.getStatusCodeMsg() + CRLF                   +
						"Content-Type: "       + response.getContentType(pt) + CRLF +
						"Content-Length: "     + response.getContentLength(pt);
			headerRes = headerRes + CRLF + CRLF;
			stage = RESBODY;
		}
	}
}

void	Post::responsBody(std::string &bodyRes)
{
	char	buff[2048];

	if (!in.is_open())
		in.open(response.getPath().c_str(), std::ios_base::binary);

	in.read(buff, sizeof(buff));
	sizeofRead     = in.gcount();
	if (sizeofRead == 0)
		in.close();
	bodyRes = std::string(buff, sizeofRead);
}
