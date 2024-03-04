/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:39:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/01 18:43:48 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Get.hpp"

Get::Get(Response &res)	:	response  (res),
							sizeofRead(0),
							saveOffset(0),
							isMoved   (false)
							
{
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

void	Get::readListOfCurDirectory()
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
		status  = "403 ";
		status += e.what();
		response.setStatusCodeMsg(status);
		throw(response.pathErrorPage("403"));
	}
}

void	Get::directoryInRequest(std::string &path, std::ifstream &file)
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	std::string 		s;

	if (path[path.length() - 1] != '/')
	{
		isMoved     = true;
		locationRes = response.getRequest().getRequestedPath() + "/";
		file.close();
		response.throwNewPath("301 Moved Permanently", "301");
	}
	if (loc.find("index")->second.empty())
	{
		if (loc.find("autoindex")->second.front().empty() or loc.find("autoindex")->second.front() == "off")
		{
			file.close();
			response.throwNewPath("403 forbidden", "403");
		}
		readListOfCurDirectory();
		return ;
	}
	response.setPath(response.concatenateIndexDirectory());
}

void	Get::statusOfFile()
{
	std::string	path;
	std::string	status;
	
	if (response.getStatusCodeMsg() == "-1")
	{
		path = response.concatenatePath( response.getRequest().getRequestedPath() );
		response.setPath(path);
	}
	std::ifstream file(response.getPath().c_str());
	if (Utils::isDir(response.getPath().c_str()))
	{
		path = response.getPath();
		directoryInRequest(path, file);
	}
	else if (!Utils::isFile(response.getPath().c_str()))
	{
		file.close();
		response.throwNewPath("404 not found", "404");
	}
	// check if location has a cgi

	status = "200 ok";
	response.UpdateStatusCode(status);
	file.close();
}

void	Get::responsHeader(std::string	&headerRes)
{
	std::string	path;

	statusOfFile();
	if (directories.empty())
	{
		path = response.getPath();
		headerRes  = response.getRequest().getProtocolVersion() + " "  +
			response.getStatusCodeMsg()                         + CRLF +
			"Content-Type: "   + response.getContentType(path)  + CRLF +
			"Content-Length: " + response.getContentLength(path);
		if (isMoved)
			headerRes = headerRes + CRLF + "Location: " + locationRes;
		headerRes = headerRes + CRLF + CRLF;
	}
	else
	{
		headerRes  = response.getRequest().getProtocolVersion() + " "  +
			response.getStatusCodeMsg()                         + CRLF +
			"Content-Type: "   + "text/html" 					+ CRLF +
			"Content-Length: " + Utils::longToString(directories.size());
		headerRes = headerRes  + CRLF + CRLF;
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

	(void)saveOffset;
	// saveOffset += sizeofRead;
}
