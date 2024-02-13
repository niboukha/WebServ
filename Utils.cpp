/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:44:08 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/12 20:37:52 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

int		Utils::isDir( const char* file )
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
