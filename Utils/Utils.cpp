/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:44:08 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/16 12:59:16 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Utils.hpp"

int	Utils::isDir( const char* file )
{
	struct stat path;
	stat(file, &path);
	
	return S_ISDIR(path.st_mode);
}

int	Utils::isFile( const char* file )
{
	struct stat path;

	stat(file, &path);
	return (S_ISREG(path.st_mode));
}

void	Utils::trimString( std::string &s )
{
	size_t	foundFisrt;
	size_t	foundLast;

	foundFisrt = s.find_last_not_of( " \t" );
	foundLast  = s.find_first_not_of( " \t" );
	if ( foundFisrt != std::string::npos )
	{
		s = s.substr( 0, foundFisrt + 1 );
		s = s.substr( foundLast );
	}
	else
		s.clear();
}

std::vector<std::string>	Utils::moreThanKey( std::string s )
{
	std::vector<std::string>	vect;
	std::string					str;

	Utils::trimString(s);
	for (size_t i = 0 ; i < s.length(); i++)
	{
		if (s[i] != ' ' and s[i] != ';')
			str += s[i];
		if ((s[i] == ' ' or s[i] == ';') and str.length() > 0)
		{
			vect.push_back(str);
			str.clear();
		}
	}
	return (vect);
}

std::string	Utils::generateRundFile()
{
	std::string			path;
	std::ostringstream	s;
	clock_t				now;
	
	now = clock();
	srand(now);
	path = "file";
	s << rand();
	return (path + s.str());
}
php
bool Utils::isFdOpen(int fd)
{
    return fcntl(fd, F_GETFD) != -1 or errno != EBADF;
}

long long	Utils::getLength(std::string s)
{
	std::stringstream	st;
	long long			n;

	st << s;
	st >> std::hex >> n;
	return (n);
}

long long	Utils::stringToLong(std::string s)
{
	std::stringstream	ss;
	long long			n;
	
	ss << s;
	ss >> n;
	return (n);
}

std::string	Utils::longToString(long long len)
{
	std::stringstream	ss;
	std::string			s;
	
	ss << len;
	ss >> s;
	return (s);
}

