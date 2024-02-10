/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:39:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/10 20:28:17 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::~Get()
{

}

Get::Get(Response &res) : 
	response(res), statusCodeMsg("-1"),
	statusMessage("ko"), path("ko")
{

}

std::string	Get::concatenatePath()
{
	mapStrVect  loc;

	loc = response.getRequest().getLocationMethod();
	return (loc["root"].front() + response.getRequest().getPathHeader());
}

int Get::isDir(const char* file)
{
    struct stat path;

    stat(file, &path);
    return S_ISREG(path.st_mode);
}

std::string	Get::concatenateIndexDirectory(std::string &file)
{
	mapStrVect  loc;
	std::string	path;
	
	loc = response.getRequest().getLocationMethod();
	for (size_t i = 0; i < loc["index"].size(); i++)
	{
		path = loc["index"][i];
		std::ifstream	myFile(loc["index"][i]);
		if (myFile.is_open())
		{
			myFile.close();
			return (loc["index"][i] + file);
		}
		myFile.close();
	}
	return (NULL);
}

const char*	Get::DirectoryFailed::what() const throw()
{
	return ("forbidden");
}

void	Get::stringOfDyrectories(std::vector<std::string> &vdir)
{	
	fillAutoIndexFile = "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>Index of /</h1>\n<hr />\n";
	for (size_t i = 0; i < vdir.size(); i++)
		fillAutoIndexFile += "<p><a href=\"" + vdir[i] + "\">" + vdir[i] + "</a></p>";
	fillAutoIndexFile += "\n</body>\n</html>\n";
}

void	Get::readListOfCurDirectory()
{
	char			cwd[PATH_MAX];
	DIR				*pDir;
	struct dirent	*pDirent;

	try
	{
		if (!getcwd(cwd, sizeof(cwd)))
			throw	DirectoryFailed();
		pDir = opendir(cwd);
		if (!pDir)
			throw	DirectoryFailed();
		while ((pDirent = readdir(pDir)))
			vDir.push_back(pDirent->d_name);
		stringOfDyrectories(vDir);
		closedir(pDir);
	}
	catch (const std::exception& e)
    {
		statusCodeMsg = "403" + e.what();
    }		
}

std::string	Get::directoryInRequest(std::string &file)
{
	mapStrVect  loc;

	loc = response.getRequest().getLocationMethod();
	if (file.back() == '/')
		statusCodeMsg = "301 Moved Permanently";
	if (loc["index"].empty())
	{
		if (loc["auto_index"].empty() or loc["auto_index"].front() == "off")
			statusCodeMsg = "403 forbidden";
		readListOfCurDirectory();
	}
	return (concatenateIndexDirectory(file));
}

void    Get::openFile()
{
	path = concatenatePath();
	std::ifstream	file(path);
	
	if (!isDir(path.c_str()))
	{
		path = directoryInRequest(path);
		if (path.empty())
			statusCodeMsg = "404 not found";
	}
	else if (!file.is_open())
		statusCodeMsg = "404 not found";
	
	//check if location has a cgi 
	statusCodeMsg = "200 ok";
	file.close();
}

std::string	Get::getContentType()
{
	size_t		found;
	std::string	s;
	std::string	ret;

	found = path.find_last_of( "." );

	if (found != std::string::npos)
	{
		s = path.substr(found + 1);
		ret = type[s];
	}
	return (ret);
}

char*	Get::response()
{
	std::string	s;

	s = response.getRequest().getProtocolVersion() + " " + 
		statusCodeMsg + "\n\r" + "Content-Type: " + response.getContentType() +
		"\n\r" + "Content-Length: " + response.contentLength(path) + "\n\r\n\r";
	return (s.c_str());
}

