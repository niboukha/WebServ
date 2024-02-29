/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:39:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/27 22:14:21 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Get.hpp"

Get::~Get()
{
}

Get::Get(Response &res)	:	response(res),
							sizeofRead(0),
							saveOffset(0),
							isMoved(false)
							
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

std::string	Get::stringOfDyrectories(std::vector<std::string> &vdir)
{
	mapStrVect	loc;
	std::string	pt;
	std::string	s;

	pt = Utils::generateRundFile();
	std::ofstream file(pt.c_str());

	file << "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>Index of /</h1>\n<hr />\n";
	for (size_t i = 0; i < vdir.size(); i++)
		file << "<p><a href=\"" + vdir[i] + "\">" + vdir[i] + "</a></p>";
	file << "\n</body>\n</html>\n";

	loc = response.getRequest().getLocation();
	s   = loc["root"].front() + "/" + pt;
	return (s);
}

std::string	Get::readListOfCurDirectory()
{
	struct dirent	*pDirent;
	DIR				*pDir;
	char			cwd[PATH_MAX];
	std::string		s;

	try
	{
		if (!getcwd(cwd, sizeof(cwd)))
			throw DirectoryFailed();

		pDir = opendir(cwd);
		if (!pDir)
			throw DirectoryFailed();

		while ((pDirent = readdir(pDir)))
			vDir.push_back(pDirent->d_name);
		
		s = "200 OK";
		response.setStatusCodeMsg(s);
		stringOfDyrectories(vDir);
		closedir(pDir);
	}
	catch (const std::exception &e)
	{
		s  = "403 ";
		s += e.what();
		response.setStatusCodeMsg(s);
		
		throw(response.pathErrorPage("403"));
	}
	return (stringOfDyrectories(vDir));
}

std::string	Get::directoryInRequest(std::string &file)
{
	mapStrVect	loc;
	std::string s;

	loc = response.getRequest().getLocation();

	if (file[file.length() - 1] != '/')
	{
		isMoved     = true;
		locationRes = response.getPath() + "/";
		response.throwNewPath("301 Moved Permanently", "301");
	}
	if (loc["index"].empty())
	{
		if (loc["autoindex"].front().empty() or loc["autoindex"].front() == "off")
			response.throwNewPath("403 forbidden", "403");
		return (readListOfCurDirectory());
	}
	return (response.concatenateIndexDirectory());
}

void	Get::statusOfFile()
{
	std::string	s;
	std::string	pt;
	
	if (response.getStatusCodeMsg() == "-1")
	{
		s = response.concatenatePath( response.getRequest().getRequestedPath() );
		response.setPath(s);
	}
	std::ifstream file(response.getPath().c_str());
	if (Utils::isDir(response.getPath().c_str()))
	{
		pt = response.getPath();
		s  = directoryInRequest(pt);
		response.setPath(s);
	}
	else if (!Utils::isFile(response.getPath().c_str()))
		response.throwNewPath("404 not found", "404");
	// check if location has a cgi

	s = "200 ok";
	response.UpdateStatusCode(s);
	file.close();
}

std::string	Get::responsHeader()
{
	std::string	s;
	std::string	pt;

	statusOfFile();
	pt = response.getPath();
	s  = response.getRequest().getProtocolVersion()       + " "  +
		response.getStatusCodeMsg()                       + CRLF +
		"Content-Type: "   + response.getContentType(pt)  + CRLF + 
		"Content-Length: " + response.getContentLength(pt);
	if (isMoved)
		s = s + CRLF + "Location: " + response.getPath();
	s = s + CRLF + CRLF;
	
	return (s);
}

std::string	Get::responsBody()
{
	std::ifstream	in(response.getPath().c_str(), std::ios_base::binary);
	char			buff[1024];

	in.seekg(saveOffset, std::ios::cur);	
	in.read(buff, sizeof(buff));
	sizeofRead  = in.gcount();
	saveOffset += sizeofRead;
	if (sizeofRead == 0)
		in.close();
	return (std::string(buff, sizeofRead));
}
