/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:39:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/14 16:50:52 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::~Get()
{

}

Get::Get(Response &res) : response(res)
{
	
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

void	Get::UpdateStatusCode(std::string &s)
{
	if (response.getStatusCodeMsg() == "-1")
		response.setStatusCodeMsg(s);
}


void	Get::readListOfCurDirectory()
{
	std::string		s;
	
	try
	{
		char			cwd[PATH_MAX];
		DIR				*pDir;
		struct dirent	*pDirent;

		if (!getcwd(cwd, sizeof(cwd)))
			throw	DirectoryFailed();
		pDir = opendir(cwd);
		if (!pDir)
			throw	DirectoryFailed();
		while ((pDirent = readdir(pDir)))
			vDir.push_back(pDirent->d_name);
		s = "200 OK";
		UpdateStatusCode(s);
		stringOfDyrectories(vDir);       //Path
		closedir(pDir);
	}
	catch (const std::exception& e)
    {
		s = "403 ";
		s += e.what();
		UpdateStatusCode(s);
		throw (response.pathErrorPage("403"));
    }		
}

std::string	Get::directoryInRequest(std::string &file)
{
	mapStrVect  loc;
	std::string	s;
	loc = response.getRequest().getLocationMethod();
	if (file.back() == '/')
	{
		s = "301 Moved Permanently";
		UpdateStatusCode(s);
		throw (response.pathErrorPage("301"));//blantha
	}
	if (loc["index"].empty())
	{
		if (loc["auto_index"].empty() or loc["auto_index"].front() == "off")
		{
			s = "403 forbidden";
			UpdateStatusCode(s);
			throw (response.pathErrorPage("403"));
		}
		readListOfCurDirectory();
	}
	return (response.concatenateIndexDirectory(file));
}

void    Get::statusOfFile()
{
	std::string		s;
	
	s = response.concatenatePath();
	response.setPath(s);
	std::ifstream	file(response.getPath());
	std::string		pt;
	
	if (!Utils::isDir(response.getPath().c_str()))
	{
		pt = response.getPath();
		s = directoryInRequest(pt);
		response.setPath(s);
		if (response.getPath().empty())
		{
			s = "404 not found";
			UpdateStatusCode(s);
			throw (response.pathErrorPage("404"));
		}
	}
	else if (!file.is_open())
	{
		s = "404 not found";
		UpdateStatusCode(s);
		throw (response.pathErrorPage("404"));
	}
	
	//check if location has a cgi
	
	s = "200 ok";
	UpdateStatusCode(s);
	file.close();
}

std::string	Get::responsHeader()
{
	std::string	s;
	std::string	pt;

	pt = response.getPath();
	s = response.getRequest().getProtocolVersion() + " " + 
		response.getStatusCodeMsg() + "\n\r" + "Content-Type: " + response.getContentType(pt) +
		"\n\r" + "Content-Length: " + response.getContentLength(pt) + "\n\r\n\r";
	return (s);
}

// std::string	Get::responsBody()
// {
	
// }
