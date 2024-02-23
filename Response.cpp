/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:39:23 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/23 08:53:37 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( Request &request ) : req( request ),
						get(NULL), post(NULL), delt(NULL),
						statusCodeMsg("-1"), path("-1")
{
}

Response::~Response()
{
	delete get;
	delete post;
	delete delt;
}

void	Response::setStatusCodeMsg(std::string& codeMsg)
{
	statusCodeMsg = codeMsg;
}

void	Response::setPath(std::string pt)
{
	path = pt;
}

void	Response::setBody(const std::string& bdy)
{
	body += bdy;
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

const std::string&	Response::getBody() const
{
	return (body);
}

const int&	Response::getfd() const
{
	return (fd);
}


std::string	Response::getContentLength( std::string &path )
{
	FILE* 				fp;
	std::stringstream	s;
	std::string			ret;
	long int			sz;

	fp = fopen(path.c_str(), "r");
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);

	s << sz;
	s >> ret;

	fclose(fp);
	return ( ret );
}

std::string		Response::getContentType( std::string &path )
{
	size_t		found;
	std::string	s;
	std::string	ret;

	mapOfTypes();
	found = path.find_last_of( "." );
	if (found != std::string::npos)
	{
		s   = path.substr(found + 1);
		ret = mimeType[s];
	}
	return ( ret );
}

void	Response::mapOfTypes( )
{
	std::ifstream	file( PATH_MIME );

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
		vec   = Utils::moreThanKey( s.substr(found + 1) );
		for ( size_t i = 0; i < vec.size(); i++ )
			mimeType[vec[i]] = value;
	}
}

std::string	Response::concatenateIndexDirectory( std::string &file )
{
	mapStrVect  loc;
	std::string	path;

	loc = getRequest().getLocation();
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
	mapStrVect	loc;
	std::string	s;

	loc = getRequest().getLocation();
	s = loc["root"].front() + getRequest().getRequestedPath();
	return (s);
}

std::string	Response::pathErrorPage(std::string code)
{
	return ((req.getServer().find(code))->second);
}

void	Response::throwNewPath(std::string msg, std::string code)
{
	std::string	s;
	
	s = msg;
	setStatusCodeMsg(s);
	throw pathErrorPage(code);
}

const std::string&	Response::getHeaderRes() const
{
	return (headerRes);
}

const std::string&	Response::getBodyRes() const
{
	return (bodyRes);
}

Stage	Response::sendResponse(Stage &stage)
{
	std::vector<std::string>	vect;

	vect.push_back("GET");
	vect.push_back("DELETE");
	vect.push_back("POST");

	size_t i = 0;
	for (; i < 3; i++) { if (!vect[i].compare(req.getMethod())) break; }

	switch(i)
	{
		case 0:
			if (get == NULL) get = new Get( *this );
			if (stage == REQBODY)
				stage = RESHEADER;
			if (stage == RESHEADER)
			{
				headerRes = get->responsHeader();
				fd = open(getPath().c_str(), O_RDWR);//tmp;
				return ( stage = RESBODY );
			}
			if (stage == RESBODY)
			{
				// if (get->getSizeofRead() != 0)
					// std::string(bodyRes += get->responsBody(), get->getSizeofRead());
					
				// std::cout << bodyRes << "\n";
				bodyRes += get->responsBody();
				// std::cout << get->getSizeofRead() << " " << std::string(get->responsBody(), get->getSizeofRead()) << "\n"; // mn b3d
				
				if (get->getSizeofRead() == 0) return ( stage = RESEND );
			}
			break;
			
		case 1:
		
			if (delt == NULL)
				delt = new Delete( *this );

			if (stage == RESHEADER)
			{
				delt->responsHeader();
				return ( stage = RESBODY );
			}
			if (delt->responsBody().length() == 0) return ( stage = RESEND );
			break;
			
		case 2:
			if (post == NULL)
		    	post = new Post( *this );
			if (stage == REQBODY || stage == RESHEADER)
			{
				post->responsHeader(stage);
				if (stage == RESHEADER)
					stage = RESBODY;
			}
			if (post->responsBody().length() == 0) return ( stage = RESEND );

		// default :
			//will define the errors that not in methods
	}
	return ( RESBODY );
}
