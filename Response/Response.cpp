/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:39:23 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/12 11:15:14 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( Request &request ) :	req( request ),
											get( NULL ),
											post( NULL ),
											delt( NULL ),
											statusCodeMsg( "-1" ),
											path( "-1" ),
											cgiStage ( INITCGI ),
											isMoved ( false )
{
	mapOfTypes();
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

void	Response::setHeaderRes(const std::string& header)
{
	headerRes = header;
}

void	Response::setLocationRes(const std::string& locRes)
{
	locationRes = locRes;
}

void	Response::setIsMoved(const bool& move)
{
	isMoved = move;
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

const std::string&	Response::getHeaderRes() const
{
	return (headerRes);
}

const std::string&	Response::getBodyRes() const
{
	return (bodyRes);
}

const std::string&	Response::getLocationRes() const
{
	return (locationRes);
}

const bool&		Response::getIsMoved() const
{
	return (isMoved);
}

void	Response::UpdateStatusCode(std::string &s)
{
	if (getStatusCodeMsg() == "-1")
		setStatusCodeMsg(s);
}

std::string	Response::getContentLength( std::string &path )
{
	long int			length;
    std::stringstream	ss;
	std::ifstream 		file(path.c_str(), std::ios::binary);

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
	std::ifstream				file( PATH_MIME );
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
		vec   = Utils::moreThanKey( s.substr(found + 1));

		for ( size_t  i = 0; i < vec.size(); i++ )
		{
			Utils::trimString(value);
			Utils::trimString(vec[i]);
			mimeType[vec[i]] = value;
			if (i == 0)
				extentionFile[value] = vec[i];
		}
	}
	file.close();
}

std::string	Response::getExtensionFile( )
{
	const std::map<std::string, std::string>	&header = getRequest().getHeaders();

	if (extentionFile.find(header.find( "content-type" )->second) != extentionFile.end())
		return ( extentionFile.find(header.find( "content-type" )->second)->second );
	return ("txt");
}

std::string		Response::getContentType( std::string &path )
{
	std::string	s;
	std::string	ret;
	size_t 		found;

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

std::string	Response::concatenatePath( std::string p )
{
	const mapStrVect	&loc = getRequest().getLocation();
	std::string	path;

	path = loc.find("root")->second.front() + p.substr(1);
	isRealPath(path);
	return (path);
}

std::string	Response::pathErrorPage(std::string code)
{
	struct stat 								statPath;
	const std::map<std::string, std::string>	&confgError = req.getServer();
	const std::map<std::string, std::string>	&serError 	= req.getErrorPages();

	if (confgError.find(code) == confgError.end()
		or (!Utils::isFile(confgError.find(code)->second.c_str())
		or   (!stat(confgError.find(code)->second.c_str(), &statPath)
		and !(statPath.st_mode & S_IWUSR))) )
			return (serError.find(code)->second);
	return (confgError.find(code)->second);
}

void	Response::throwNewPath(std::string msg, std::string code)
{
	std::string	status;
	
	status = msg;
	setStatusCodeMsg(status);
	throw pathErrorPage(code);
}

Stage	Response::sendResponse(Stage &stage, std::string &reqBuff)
{
	std::vector<std::string>	vect;

	vect.push_back("GET");
	vect.push_back("DELETE");
	vect.push_back("POST");

	int  i = 0;
	for (; i < 3; i++) { if (!vect[i].compare(req.getMethod())) break; }
	switch(i)
	{
		case 0:
			if (get   == NULL)		get = new Get( *this );
			if (stage == REQBODY)	return (stage = RESHEADER);
			if (stage == RESHEADER)
			{
				get->responsHeader(headerRes, stage, cgiStage);
				return ( stage );
			}
			if (stage == RESBODY)
			{
				get->responsBody(bodyRes);

				if (get->getSizeofRead() == 0)
					return ( stage = RESEND );
			}
			break;

		case 1:

			if (delt  == NULL) delt = new Delete( *this );
			if (stage == REQBODY)	return (stage = RESHEADER);
			if (stage == RESHEADER)
			{
				delt->responsHeader(headerRes);
				return ( stage = RESBODY );
			}
			if (stage == RESBODY)
			{
				delt->responsBody(bodyRes);
		
				if (delt->getSizeofRead() == 0)
					return ( stage = RESEND );
			}
			break;

		case 2:

			if (post  == NULL) post = new Post( *this );
			if (stage == REQBODY)
			{
				post->requestedStatus(stage, reqBuff, cgiStage);
				return (stage);
			}
			if (stage == RESHEADER)
			{
				post->responsHeader(stage, reqBuff, headerRes, cgiStage);
				return ( stage = RESBODY );
			}
			if (stage == RESBODY)
			{
				post->responsBody(bodyRes);
				if (post->getSizeofRead() == 0) return ( stage = RESEND );
			}
			break;
	}
	return ( RESBODY );
}
