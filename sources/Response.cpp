/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:39:23 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/28 07:09:53 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

Response::Response( Request &request ) :	req( request ),
											get( NULL ),
											post( NULL ),
											delt( NULL ),
											statusCodeMsg( "-1" ),
											path( "-1" ),
											uploadLength(0 )
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
	// std::cout << bdy.size() << "_" << body.size() << "\n";
	body = bdy;
}

void	Response::setUploadLength(long long  b)
{
	uploadLength = b;
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

const std::string&	Response::getHeaderRes() const
{
	return (headerRes);
}

const std::string&	Response::getBodyRes() const
{
	return (bodyRes);
}

const long long &		Response::getUploadLength() const
{
	return (uploadLength);
}

void	Response::UpdateStatusCode(std::string &s)
{
	if (getStatusCodeMsg() == "-1")
		setStatusCodeMsg(s);
}

std::string	Response::getContentLength( std::string &path )
{
    std::stringstream	ss;
	long int			length;
	
	std::ifstream file(path.c_str(), std::ios::binary);

    file.seekg(0, std::ios::end);
    length = file.tellg();
    if (length == -1)
		return ("0");
    ss << length;
	file.close();
    return (ss.str());
}

void	Response::mapOfTypes( )
{
	std::ifstream	file( PATH_MIME );

	std::vector<std::string>	vec;
	std::string					s;
	std::string					value;
	size_t 						found;

	std::getline(file, s);
	while ( std::getline( file, s ) )
	{
		Utils::trimString( s );
		found = s.find_first_of( " \t" );
		value = s.substr( 0, found + 1 );
		vec   = Utils::moreThanKey( s.substr(found + 1) );

		for ( size_t  i = 0; i < vec.size(); i++ )
			mimeType[vec[i]] = value;
	}
}

std::string	Response::getExtensionFile()
{
	std::map<std::string, std::string>				header;
	std::map<std::string, std::string> ::iterator	it;

	Response::mapOfTypes( );
	header = getRequest().getHeaders();
	for(it = mimeType.begin(); it != mimeType.end(); it++)
	{
		Utils::trimString( it->second );
		if (header["content-type"] == it->second )
			return (it->first);
	}
	return ("txt");
}

std::string		Response::getContentType( std::string &path )
{
	size_t 		found;
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

std::string	Response::concatenateIndexDirectory( )
{
	mapStrVect  loc;
	
	loc = getRequest().getLocation();

	for (size_t  i = 0; i < loc["index"].size(); i++)
	{
		std::ifstream	myFile(loc["index"][i].c_str());

		if (myFile.is_open())
		{
			myFile.close();
			return (loc["root"].front() + "/" + loc["index"][i]);
		}
		myFile.close();
	}
	throwNewPath("404 forbidden", "404");
	return (NULL);
}

void	Response::isRealPath(std::string &path)
{
	char		bufRec[PATH_MAX];
	char		bufCur[PATH_MAX];
	char		*resource;
	char		*currentPath;
	std::string	res;
	std::string	curr;

	resource    = realpath(path.c_str(), bufRec);
	currentPath = realpath(".", bufCur);

	if (resource)
	{
		res			= bufRec;
		curr		= bufCur;
		if (res.find(curr) == std::string::npos)
		{
			res = "403 forbidden";
			setStatusCodeMsg(res);
			throw (pathErrorPage("403"));	
		}
		if (path[path.length() - 1] == '/')
			path = res + "/";
	}
}

std::string	Response::concatenatePath( std::string p ) //real path 
{
	mapStrVect	loc;
	std::string	s;

	loc = getRequest().getLocation();
	s   = loc["root"].front() + p;
	isRealPath(s);
	return (s);
}

std::string	Response::pathErrorPage(std::string code)
{
	std::map<std::string, std::string>	confgError;
	std::map<std::string, std::string>	serError;
	std::string							sE;
	struct stat 						statPath;
	
	confgError = req.getServer();
	serError   = req.getErrorPages();
	sE         = confgError[code];
	
	if (!Utils::isFile(sE.c_str())
		or (!stat(sE.c_str(), &statPath)
		and !(statPath.st_mode & S_IWUSR)) )
			return (serError[code]);
	return (confgError[code]);
}

void	Response::throwNewPath(std::string msg, std::string code)
{
	std::string	s;
	
	s = msg;
	setStatusCodeMsg(s);
	throw pathErrorPage(code);
}

Stage	Response::sendResponse(Stage &stage)
{
	std::vector<std::string>	vect;

	vect.push_back("GET");
	vect.push_back("DELETE");
	vect.push_back("POST");

	long long  i = 0;
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
				return ( stage = RESBODY );
			}
			if (stage == RESBODY)
			{
				bodyRes += get->responsBody();
				if (get->getSizeofRead() == 0) return ( stage = RESEND );
			}
			break;

		case 1:

			if (delt == NULL)
				delt = new Delete( *this );
			if (stage == REQBODY)
				stage = RESHEADER;
			if (stage == RESHEADER)
			{
				headerRes = delt->responsHeader();
				return ( stage = RESBODY );
			}
			if (stage == RESBODY)
			{
				bodyRes += delt->responsBody();
				if (delt->getSizeofRead() == 0)
					return ( stage = RESEND );
			}
			break;

		case 2:

			if (post == NULL)
			{
		    	post = new Post( *this );
			}
			if (stage == REQBODY)
			{
				post->requestedStatus(stage);
				// std::cout << "stage in response : " << stage << "\n";
				return (stage);
			}
			if (stage == RESHEADER)
			{
				// std::cout << "stage in head : " << stage << "\n";
				headerRes = post->responsHeader(stage);
				return ( stage = RESBODY );
			}
			if (stage == RESBODY)
			{
				bodyRes += post->responsBody();
				if (post->getSizeofRead() == 0) return ( stage = RESEND );
			}
			break;
		// default :
			//will define the errors that not in methods
	}
	return ( RESBODY );
}
