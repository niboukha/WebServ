/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:35:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/26 20:00:18 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Delete.hpp"

Delete::Delete(Response &response) : res(response), sizeofRead(0)
{
}

Delete::~Delete()
{
}

const int&		Delete::getSizeofRead() const
{
	return (sizeofRead);
}

void	Delete::pathOfSentPage(std::string s, std::string code)
{
	res.setStatusCodeMsg(s);
	res.setPath(res.pathErrorPage(code));
}

void Delete::directoryPath(struct stat st, std::string &pt)
{
	if (pt[pt.length() - 1] != '/')
	{
		pathOfSentPage("409 conflict", "409");
		return ;
	}

	if (!(st.st_mode & S_IWUSR))
	{
		pathOfSentPage("403 Forbidden", "403");
		return ;
	}

	if ((st.st_mode & S_IWUSR) and std::remove(pt.c_str()))
		pathOfSentPage("500 Internal Server Error", "500");
	else
		pathOfSentPage("204 No Content", "204");
}

void	Delete::filePath(std::string &s)
{
	if (!std::remove(s.c_str()))
		pathOfSentPage("204 No Content", "204");
}

std::string	Delete::nestedDirectories(std::string s, struct stat statPath)
{
	DIR				*pDir;
	struct dirent	*pDirent;
	std::string		str;

	if ((pDir = opendir(s.c_str())) == NULL)
		return (s);
	str  = s;
	while ((pDirent = readdir(pDir)))
	{
		if (strlen(pDirent->d_name) == 0)
			break;
		s = pDirent->d_name;
		if (s == "." or s == "..")
			continue;

		s = str + pDirent->d_name;
		if (!stat(s.c_str(), &statPath))
		{
			if (S_ISDIR(statPath.st_mode))
			{
				s += "/";
				nestedDirectories(s, statPath) = s;
				directoryPath(statPath, s);
			}
			else if (S_ISREG(statPath.st_mode))
				filePath(s);
		}
		else
			pathOfSentPage("404 Not found", "404");
	}
	closedir(pDir);
	return (str);
}

void	Delete::deleteBasePath(std::string s, struct stat statPath)
{
	if (!stat(s.c_str(), &statPath))
	{
		if (S_ISDIR(statPath.st_mode))
			directoryPath(statPath, s);
		else if (S_ISREG(statPath.st_mode))
			filePath(s);
	}
	else
		pathOfSentPage("404 Not found", "404");
}

void	Delete::statusOfRequested()
{
	std::string	pt;
	struct stat	statPath;
	
	pt = res.concatenatePath( res.getRequest().getRequestedPath() );
	res.setPath(pt);
	pt = res.getPath();
	
	nestedDirectories(pt, statPath);
	res.setPath(pt);
	deleteBasePath(pt, statPath);
}

std::string	Delete::responsHeader()
{
	std::string	s;
	std::string	pt;

	statusOfRequested();
	pt = res.getPath();
	s  = res.getRequest().getProtocolVersion() + " " +
		res.getStatusCodeMsg() + CRLF +
		"Content-Type: "   + res.getContentType(pt)   + CRLF +
		"Content-Length: " + res.getContentLength(pt) + CRLF +
		CRLF;
	return (s);
}

std::string	Delete::responsBody()
{
	char buffer[20];

	if (!Utils::isFdOpen(fd))
		fd = open(res.getPath().c_str(), O_RDWR);
	sizeofRead = read(fd, buffer, sizeof(buffer));
	
	return (std::string(buffer, sizeofRead));
}
