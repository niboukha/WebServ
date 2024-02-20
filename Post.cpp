/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 18:32:06 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 18:34:56 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post( Response &response ) : res( response ), size(0)
{

}

Post::~Post( )
{

}

void	Post::uploadedFile() // check getuploadPass in another fun
{
	mapStrVect	loc;

	loc = res.getRequest().getLocation();

	if (loc.find("Content-Length") != loc.end())
		nonChunkedTransfer();
	chunkedTransfer();
}

std::string	Post::conctRootUpload(std::string s)
{
	mapStrVect	loc;
	std::string	pt;

	loc = res.getRequest().getLocation();
	pt = loc["root"].front() + s + "/";
	return (pt);
}

void	Post::isUploadPass()
{
	mapStrVect	loc;
	std::string	s;

	loc = res.getRequest().getLocation();

	if (loc["upload-pass"].empty())
		throw (res.concatenatePath());

	s  = conctRootUpload(loc["upload-pass"].front());
	s += Utils::generateRundFile();
	res.setPath(s);
}

void	Post::nonChunkedTransfer()
{
	std::map<std::string, std::string>	ser;

	mapStrVect			head;
	std::stringstream	s;
	std::ofstream		outfile;
	size_t				n;	

	s << head["Content-Length"].front();
	s >> n;
	ser  = res.getRequest().getServer();
	head = res.getRequest().getHeaders(); 
	if (ser["client_max_body_size"] > head["Content-Length"].front())
		throw "413 Request Entity Too Large";

	outfile.open(res.getPath().c_str(), std::ios_base::app);
	size += res.getBody().length();
	outfile << res.getBody();
	if (size == n)
		outfile.close();
}

void	Post::chunkedTransfer()
{
	std::ofstream	outfile;
	int				len;
	std::string		lrn;
	size_t			found;

	outfile.open(res.getPath().c_str(), std::ios_base::app);

	found = res.getBody().find("\r\n");
	if (found != std::string::npos)
	{
		lrn = res.getBody().substr(0, found);
	}

}

void	Post::requestedStatus()
{
	try
	{
		mapStrVect	header;

		header = res.getRequest().getHeaders();
		isUploadPass();

		if (header.find("Content-Length") != header.end())
			nonChunkedTransfer();
		chunkedTransfer();
	}
	catch (std::string	s)
	{
		res.setPath(s);
		
	}
}


