/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:35:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/22 18:54:13 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete(Response &response) : res(response)
{
}

Delete::~Delete()
{
}

void Delete::WriteAccess()
{
	struct stat st;
	std::string s;

	if (st.st_mode & S_IWUSR)
		res.throwNewPath("500 Internal Server Error", "500");
	else
		res.throwNewPath("403 Forbidden", "403");
}

void Delete::directoryPath(struct stat st, std::string &pt)
{
	std::string err;

	if (pt[pt.length() - 1] != '/')
		res.throwNewPath("409 conflict", "409");
		
	if (!(st.st_mode & S_IWUSR))
		res.throwNewPath("403 Forbidden", "403");
		
	if ((st.st_mode & S_IWUSR) && std::remove(res.getPath().c_str()))
		res.throwNewPath("500 Internal Server Error", "500");
	else
		res.throwNewPath("204 No Content", "204");
}

void	Delete::filePath(std::string &s)
{
	std::string err;

	if (!std::remove(s.c_str()))
		res.throwNewPath("204 No Content", "204");
}

std::string	Delete::nestedDirectories(std::string s, struct stat statPath)
{
	DIR				*pDir;
	struct dirent	*pDirent;
	std::string		str;

	pDir = opendir(s.c_str());
	str  = s;

	while ((pDirent = readdir(pDir)))
	{
		s = pDirent->d_name;

		if (s[0] == '.')
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
			res.throwNewPath("404 Not found", "404");
	}
	return (s);
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
		res.throwNewPath("404 Not found", "404");
}

void	Delete::statusOfRequested()
{
	std::string	pt;
	struct stat	statPath;

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
		"Content-Length: " + res.getContentLength(pt) + CRLF + // wish content length (dleted file or something else)
		CRLF;
	return (s);
}

std::string	Delete::responsBody()
{
	char buffer[1024];

	fd = open(res.getPath().c_str(), O_RDWR);

	read(fd, buffer, sizeof(buffer));
	return (buffer);
}
