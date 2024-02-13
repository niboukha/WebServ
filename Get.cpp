/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:39:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/12 22:08:52 by niboukha         ###   ########.fr       */
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

void	Get::readListOfCurDirectory()
{
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
		statusCodeMsg = "200 OK";
		stringOfDyrectories(vDir);       //Path
		closedir(pDir);
	}
	catch (const std::exception& e)
    {
		statusCodeMsg = "403";
		statusCodeMsg += e.what();
		//
    }		
}

std::string	Get::directoryInRequest(std::string &file)
{
	mapStrVect  loc;

	loc = response.getRequest().getLocationMethod();
	if (file.back() == '/')
		statusCodeMsg = "301 Moved Permanently"; return ; //
	if (loc["index"].empty())
	{
		if (loc["auto_index"].empty() or loc["auto_index"].front() == "off")
			statusCodeMsg = "403 forbidden"; return ; //
		readListOfCurDirectory();
	}
	return (response.concatenateIndexDirectory(file));
}

void    Get::statusOfFile()
{
	path = response.concatenatePath();
	std::ifstream	file(path);
	
	if (!Utils::isDir(path.c_str()))
	{
		path = directoryInRequest(path);
		if (path.empty())
			statusCodeMsg = "404 not found"; return ;//
	}
	else if (!file.is_open())
		statusCodeMsg = "404 not found"; return ;//
	
	//check if location has a cgi
	
	statusCodeMsg = "200 ok";
	file.close();
}

std::string	Get::responsHeader()
{
	std::string	s;

	s = response.getRequest().getProtocolVersion() + " " + 
		statusCodeMsg + "\n\r" + "Content-Type: " + response.getContentType(path) +
		"\n\r" + "Content-Length: " + response.getContentLength(path) + "\n\r\n\r";
	return (s);
}

std::string	Get::responsBody()
{
	
}
