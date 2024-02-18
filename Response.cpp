/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:39:23 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 12:50:08 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( Request &request ) : req( request ), get(NULL), delt(NULL)
{

}

Response::~Response()
{

}

void	Response::setStatusCodeMsg(std::string& statusCodeMsg)
{
	statusCodeMsg = "-1";
}

void	Response::setPath(std::string& path)
{
	path = "-1";
}

const Request&	Response::getRequest() const
{
	return ( req );
}

const std::string&	Response::getStatusCodeMsg() const
{
	return (statusCodeMsg);
}

const std::string&	Response::getPath() const
{
	return (path);
}

std::string	Response::getContentLength( std::string &path )
{
	long int			sz;
	std::stringstream	s;
	std::string			ret;

	FILE* fp;

	fp = fopen(path.c_str(), "r");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
	s << sz;
	s >> ret;
	return ( ret );
}

std::string		Response::getContentType( std::string &path )
{
	size_t		found;
	std::string	s;
	std::string	ret;

	found = path.find_last_of( "." );

	if (found != std::string::npos)
	{
		s = path.substr(found + 1);
		ret = contType[s];
	}
	return ( ret );
}

void	Response::mapOfTypes( )
{
	std::ifstream	file( "/nfs/homes/niboukha/Desktop/webServ/mimetype.txt" );

	std::vector<std::string>	vec;
	std::string					s;
	std::string					value;
	size_t						found;

	std::getline(file, s);
	while ( std::getline( file, s ) )
	{
		Utils::trimString( s );
		found = s.find_first_of( " \t" );
		value = s.substr( 0, found + 1 );
		vec = Utils::moreThanKey( s.substr(found + 1) );
		for ( size_t i = 0; i < vec.size(); i++ )
			contType[vec[i]] = value;
	}
}

std::string	Response::concatenateIndexDirectory( std::string &file )
{
	mapStrVect  loc;
	std::string	path;

	loc = getRequest().getLocationMethod();
	for (size_t i = 0; i < loc["index"].size(); i++)
	{
		path = loc["index"][i];
		std::ifstream	myFile(loc["index"][i].c_str());
		if (myFile.is_open())
		{
			myFile.close();
			return (loc["index"][i] + file);
		}
		myFile.close();
	}
	return (NULL);
}

std::string	Response::concatenatePath( )
{
	mapStrVect  loc;
	std::string	s;
	loc = getRequest().getLocationMethod();

	s = loc["root"].front() + getRequest().getPathHeader();
	return (s);
}

std::string	Response::pathErrorPage(std::string code)
{
	std::map<std::string, std::string>	err;
	std::string							path;

	err = req.getErrorPage();
	path = err[code];
	return (path);
}

void	Response::prefaceMethod( )
{
	if (req.getMethod() == "Get")
	{
		if (get == NULL)
			get = new Get( *this );//up
	}
	else if (req.getMethod() == "Delete")
	{
		if (delt == NULL)
			delt = new Delete(*this);
	}
	// else if (req.getMethod() == "Post")
	//     post = new Post(*this);
}

Stage	Response::sendResponse(int stage)
{
	if (stage == RESHEADER)
	{
		get->responsHeader();
		return ( RESBODY );
	}
	if (get->responsBody().length() == 0)
		return ( RESEND );
	return ( RESBODY );
}

