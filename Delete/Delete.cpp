/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:35:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/13 11:30:41 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete( Response &response ) :	res		  (response),
										sizeofRead(0),
										isReal	  (true)
{
}

Delete::~Delete( )
{
}

const std::streampos&	Delete::getSizeofRead( ) const
{
	return (sizeofRead);
}

void	Delete::pathOfSentPage( std::string s, std::string code )
{
	res.setStatusCodeMsg(s);
	throw ( code );
}

void	Delete::directoryPath( DIR *pDir, struct stat st, std::string &pt )
{
	std::string	s;

	if (pt[pt.length() - 1] != '/')
	{
		closedir(pDir);
		pathOfSentPage("409 conflict", "409");
	}

	if (!(st.st_mode & S_IWUSR))
	{
		closedir(pDir);
		pathOfSentPage("403 Forbidden", "403");
	}

	if ((st.st_mode & S_IWUSR) and std::remove(pt.c_str()))
	{
		closedir(pDir);
		pathOfSentPage("500 Internal Server Error", "500");
	}
}

void	Delete::filePath( std::string &s )
{
	std::string	set;
	
	if (!std::remove(s.c_str()))
	{
		set = "204 No Content";
		res.setStatusCodeMsg(set);
		res.setPath(res.pathErrorPage("204"));
	}
}

std::string	Delete::nestedDirectories( std::string s, struct stat statPath )
{
	struct dirent	*pDirent;
	DIR				*pDir;
	std::string		str;
	std::string		set;

	if (!(pDir = opendir(s.c_str())))
		return (s);

	str = s;
	while ((pDirent = readdir(pDir)))
	{
		if (!strlen(pDirent->d_name))
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
				directoryPath(pDir, statPath, s);
			}
			else if (S_ISREG(statPath.st_mode))
				filePath(s);
		}
		else
		{
			closedir(pDir);
			pathOfSentPage("404 Not found", "404");
		} 
	}
	closedir(pDir);
	return (str);
}

void	Delete::deleteBasePath( std::string s, struct stat statPath )
{
	std::string	set;
	
	if (!stat(s.c_str(), &statPath))
	{
		if (S_ISDIR(statPath.st_mode))
		{
			if (s[s.length() - 1] != '/')
				pathOfSentPage("409 conflict", "409");

			if (!(statPath.st_mode & S_IWUSR))
				pathOfSentPage("403 Forbidden", "403");

			if ((statPath.st_mode & S_IWUSR) and std::remove(s.c_str()))
				pathOfSentPage("500 Internal Server Error", "500");
			else
				pathOfSentPage("204 No Content", "204");
		}
		else if (S_ISREG(statPath.st_mode))
		{
			if (!std::remove(s.c_str()))
				pathOfSentPage("204 No Content", "204");
		}
	}
	else
		pathOfSentPage("404 Not found", "404");
}

void	Delete::statusOfRequested( )
{
	std::string	base;
	struct stat	statPath;
	
	try
	{
		if (res.getStatusCodeMsg() == "-1")
		{
			res.setPath (res.concatenatePath( res.getRequest().getRequestedPath() ));
			isReal = false;
		}
		base = res.getPath();
		if (!stat(base.c_str(), &statPath))
		{
			if (S_ISDIR(statPath.st_mode))
			{
				if (base[base.length() - 1] != '/')
					pathOfSentPage("409 conflict", "409");
				if (!(statPath.st_mode & S_IWUSR))
					pathOfSentPage("403 Forbidden", "403");
			}
		}
		nestedDirectories (res.getPath(), statPath);
		res.setPath		  (base);
		deleteBasePath    (res.getPath(), statPath);
	}
	catch(std::string code)
	{
		if (isReal)
			res.setPath (code);
		else
			res.setPath(res.pathErrorPage(code));
	}
}

void	Delete::responsHeader(std::string &headerRes)
{
	std::string	pt;
	
	statusOfRequested();

	pt         = res.getPath();
	headerRes  = res.getRequest().getProtocolVersion()         + " "  +
				 res.getStatusCodeMsg()                        + CRLF +
		 		 "Content-Type: "   + res.getContentType(pt)   + CRLF +
				 "Content-Length: " + res.getContentLength(pt) + CRLF +
				 CRLF;
}

void	Delete::responsBody(std::string &bodyRes)
{
	char	buff[2048];

	if (!in.is_open())
		in.open(res.getPath().c_str(), std::ios_base::binary);

	in.read(buff, sizeof(buff));
	sizeofRead  = in.gcount();
	if (sizeofRead == 0)
		in.close();
	bodyRes = std::string(buff, sizeofRead);
}
