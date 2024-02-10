/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:39:23 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/10 11:33:22 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request &request) : req(request)
{
	contentType(type);
}

Response::~Response()
{
    
}

const Request	Response::getRequest() const
{
	return (req);
}

const std::map<std::string, std::string>	Response::getType()
{
	return (type);
}


void   Response::fillResponse()
{
	if (req.getMethod() == "Get")
	{
		get = new Get(*this);
	}    
	// else if (req.getMethod() == "Post")
	//     post = new Post(*this);
	// else if (req.getMethod() == "Delete")
	//     delt = new Delete(*this);


}

void	Response::trimString( std::string &s )
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

std::vector<std::string>	Response::moreThanKey( std::string s )
{
	std::vector<std::string>	vect;
	std::string					str;

	trimString(s);
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

void	Response::contentType(std::map<std::string, std::string>	&tP)
{
	std::ifstream	file( "/nfs/homes/niboukha/Desktop/webServ/mimetype.txt" );

	std::vector<std::string>	vec;
	std::string					s;
	std::string					value;
	size_t						found;

	std::getline(file, s);
	while ( std::getline( file, s ) )
	{
		trimString( s );
		found = s.find_first_of( " \t" );
		value = s.substr( 0, found + 1 );
		vec = moreThanKey( s.substr(found + 1) );
		for ( size_t i = 0; i < vec.size(); i++ )
			tP[vec[i]] = value;
	}
}

long int Response::contentLength(std::string path)
{
	long int	sz;

	FILE* fp = fopen(path.c_str(), "r");
    fseek(fp, 0L, SEEK_END); 
    sz = ftell(fp); 
    fclose(fp); 
	return (sz);
}

