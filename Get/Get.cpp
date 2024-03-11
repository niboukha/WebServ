/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:39:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/11 19:02:19 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Get.hpp"
#include "../Response/Response.hpp"

Get::Get(Response &res)	:	response  (res),
							cgi       (res),
							sizeofRead(0),
							isMoved   ( false )
							
{
	// cgi.fillEnvirement();
}

Get::~Get()
{
}

const char*	Get::DirectoryFailed::what( ) const throw( )
{
	return ("forbidden");
}

const std::streampos&	Get::getSizeofRead() const
{
	return (sizeofRead);
}

void	Get::stringOfDyrectories(std::vector<std::string> &vdir)
{
	directories      =  "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>Index of /</h1>\n<hr />\n";
	for (size_t i    = 0; i < vdir.size(); i++)
		directories +=  "<p><a href=\"" + vdir[i] + "\">" + vdir[i] + "</a></p>";
	directories     +=  "\n</body>\n</html>\n";
}

void	Get::readListOfCurDirectory( CgiStage &cgiStage)
{
	struct dirent	*pDirent;
	DIR				*pDir;
	std::string		status;

	try
	{
		pDir = opendir(response.getPath().c_str());
		if (!pDir)
			throw DirectoryFailed();

		while ((pDirent = readdir(pDir)))
			vDir.push_back(pDirent->d_name);

		status = "200 OK";
		response.setStatusCodeMsg(status);

		stringOfDyrectories(vDir);
		closedir(pDir);
	}
	catch (const std::exception &e)
	{
		status   = "403 ";
		status  += e.what();
		response.setStatusCodeMsg(status);
		cgiStage = ERRORCGI;
		throw(response.pathErrorPage("403"));
	}
}

void	Get::directoryInRequest(std::string &path, std::ifstream &file, CgiStage &cgiStage)
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	std::string 		s;

	if (path[path.length() - 1] != '/')
	{
		isMoved     = true;
		locationRes = response.getRequest().getRequestedPath() + "/";
		cgiStage    = ERRORCGI;
		file.close();
		response.throwNewPath("301 Moved Permanently", "301");
	}
	if (loc.find("index")->second.front().empty())
	{
		if (loc.find("autoindex")->second.front().empty() or loc.find("autoindex")->second.front() == "off")
		{
			file.close();
			cgiStage = ERRORCGI;
			response.throwNewPath("403 forbidden", "403");
		}
		readListOfCurDirectory(cgiStage);
		return ;
	}
	response.setPath(response.concatenateIndexDirectory());
}

void	Get::pathPermission( CgiStage &cgiStage )
{
	struct stat statPath;

	if (!stat(response.getPath().c_str(), &statPath))
		if (!(statPath.st_mode & S_IWUSR))
		{
			cgiStage = ERRORCGI;
			response.throwNewPath("403 Forbidden", "403");
		}
}

bool	Get::cgiPassCheckment( CgiStage &cgiStage )
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	
	if(loc.find("cgi_pass") != loc.end() && !loc.find("cgi_pass")->second.front().empty())
	{
		if ((loc.find("cgi_pass")->second.front() != ".py" 
			and loc.find("cgi_pass")->second.front() != ".php") and cgiStage != ERRORCGI)
		{
			cgiStage = ERRORCGI;
			response.throwNewPath("403 forbidden", "403"); //check
		}
		return (true);
	}
	return (false);
}

void	Get::statusOfFile(Stage& stage, CgiStage &cgiStage)
{
	std::string			path;
	std::string			status;
	std::string			s;

	if (response.getStatusCodeMsg() == "-1")
	{
		path = response.concatenatePath( response.getRequest().getRequestedPath() );
		response.setPath(path);
	}
	std::ifstream file(response.getPath().c_str());
	if (Utils::isDir(response.getPath().c_str()))
	{
		pathPermission(cgiStage);
		path = response.getPath();
		directoryInRequest(path, file, cgiStage);
	}
	else if (!Utils::isFile(response.getPath().c_str()))
	{
		file.close();
		cgiStage = ERRORCGI;
		response.throwNewPath("404 not found", "404");
	}
	pathPermission(cgiStage);
	if (cgiPassCheckment(cgiStage) && cgiStage == INITCGI)
	{
		cgiStage = WAITCGI;
		cgi.executeCgi(s, stage, cgiStage);
	}
	else
	{
		status   = "200 ok";
		response.UpdateStatusCode(status);
	}
	file.close();
}

void	Get::responsHeader(std::string	&headerRes, Stage& stage, CgiStage	&cgiStage)
{
	if (cgiStage == WAITCGI)
		cgi.waitCgi(stage, cgi.getPid(), cgiStage);
	else
	{
		std::string			path;

		statusOfFile(stage, cgiStage);
		if (cgiStage == EXECUTECGI)
		{
			headerRes  = response.getRequest().getProtocolVersion() + " "  +
				response.getStatusCodeMsg()                         + CRLF;
			stage = RESBODY;
			return ;
		}
		
		if (directories.empty())
		{
			path       = response.getPath();
			headerRes  = response.getRequest().getProtocolVersion() + " "  +
				response.getStatusCodeMsg()                         + CRLF +
				"Content-Type: "   + response.getContentType(path)  + CRLF +
				"Content-Length: " + response.getContentLength(path);
			if (isMoved)
				headerRes = headerRes + CRLF + "Location: " + locationRes;
			headerRes     = headerRes + CRLF + CRLF;
			stage = RESBODY;
		}
		else
		{
			headerRes  = response.getRequest().getProtocolVersion() + " "  +
				response.getStatusCodeMsg()                         + CRLF +
				"Content-Type: "   + "text/html" 					+ CRLF +
				"Content-Length: " + Utils::longToString(directories.size());
			headerRes = headerRes  + CRLF + CRLF;
			stage = RESBODY;
		}
	}
}

void	Get::responsBody(std::string &bodyRes)
{
	if (directories.empty())
	{
		char	buff[2048];
		if (!in.is_open())
			in.open(response.getPath().c_str(), std::ios_base::binary);

		in.read(buff, sizeof(buff));
		sizeofRead  = in.gcount();
		if (sizeofRead == 0)
			in.close();
		bodyRes = std::string(buff, sizeofRead);
	}
	else
	{
		if (directories.size() > 2048)
		{
			bodyRes = std::string (directories, 0, 2048);
			directories = std::string (directories, 2048 + 1);
			sizeofRead = 2048;
		}
		else
		{
			bodyRes = directories;
			sizeofRead = directories.size();
			directories.clear();
		}
	}
}
