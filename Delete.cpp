/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:35:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 15:59:02 by niboukha         ###   ########.fr       */
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
	{
		s = "500 Internal Server Error";
		res.setStatusCodeMsg(s);
		throw(res.pathErrorPage("500"));
	}
	else
	{
		s = "403 Forbidden";
		res.setStatusCodeMsg(s);
		throw(res.pathErrorPage("403"));
	}
}

void Delete::directoryPath(struct stat st, std::string &pt)
{
	std::string err;

	if (pt[pt.length() - 1] != '/')
	{
		err = "409 conflict";
		res.setStatusCodeMsg(err);
		throw(res.pathErrorPage("409"));
	}
	if (!(st.st_mode & S_IWUSR))
	{
		err = "403 Forbidden";
		res.setStatusCodeMsg(err);
		throw(res.pathErrorPage("403"));
	}
	if ((st.st_mode & S_IWUSR) && std::remove(res.getPath().c_str()))
	{
		err = "500 Internal Server Error";
		res.setStatusCodeMsg(err);
		throw(res.pathErrorPage("500"));
	}
	else
	{
		err = "204 No Content";
		res.setStatusCodeMsg(err);
		throw(res.pathErrorPage("204"));

	}
}

void	Delete::filePath(std::string &s)
{
	std::string err;

	if (!std::remove(s.c_str()))
	{
		err = "204 No Content";
		res.setStatusCodeMsg(err);
		throw(res.pathErrorPage("204"));
	}
}

std::string	Delete::nestedDirectories(std::string s, struct stat statPath)
{
	DIR				*pDir;
	struct dirent	*pDirent;
	std::string		str;

	pDir = opendir(s.c_str());
	str = s;
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
		{
			s = "404 Not found";
			res.setStatusCodeMsg(s);
			throw(res.pathErrorPage("404"));
		}
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
	{
		s = "404 Not found";
		res.setStatusCodeMsg(s);
		throw(res.pathErrorPage("404"));
	}
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
	s = res.getRequest().getProtocolVersion() + " " +
		res.getStatusCodeMsg() + CARIAGE_RETURN +
		"Content-Type: " + res.getContentType(pt) + CARIAGE_RETURN +
		"Content-Length: " + res.getContentLength(pt) + CARIAGE_RETURN +
		CARIAGE_RETURN;
	return (s);
}

std::string	Delete::responsBody()
{

}
