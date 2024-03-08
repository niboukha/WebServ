/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:57:58 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/08 09:59:24 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi(Response &res) : response(res)
{
	// linkReqEnv();
}

Cgi::~Cgi()
{
}

// void	Cgi::linkReqEnv()
// {
// 	const std::map<std::string, std::string>	&header = response.getRequest().getHeaders();
// 	if (header.find("content-type") != header.end())
// 		requestEnv["CONTENT_TYPE"] = header.find("content-type")->second;

// 	if (header.find("content-length") != header.end())
// 		requestEnv["CONTENT_LENGTH"] = header.find("content-length")->second;

// 	if (header.find("cookie") != header.end())
// 		requestEnv["HTTP_COOKIE"] = header.find("cookie")->second;

// 	if (header.find("user-agent") != header.end())
// 		requestEnv["HTTP_USER_AGENT"] = header.find("user-agent")->second;

// 	if (!response.getRequest().getQueryParameters().empty())
// 		requestEnv["QUERY_STRING"] = response.getRequest().getQueryParameters();
	
// 	if (!response.getRequest().getMethod().empty())
// 		requestEnv["REQUEST_METHOD"] = response.getRequest().getMethod();

// 	std::cout << requestEnv["REQUEST_METHOD"] << "\n";

// 	// if (header.find("content-type") != header.end())
// 	// 	requestEnv["REMOTE_ADDR"] = header.find("content-type")->second;

// 	// if (header.find("content-type") != header.end())
// 	// 	requestEnv["REMOTE_HOST"] = header.find("content-type")->second;

// 	// if (header.find("content-type") != header.end())
// 	// 	requestEnv["PATH_INFO"] = header.find("content-type")->second;
// }

void	Cgi::fillEnvirement()
{
	// std::string	keyval;
	// int			i;
	
	// env = new char*[requestEnv.size()];

	// i = 0;
	// for(std::map<std::string, std::string> :: iterator it = requestEnv.begin(); it != requestEnv.end(); it++)
	// {
	// 	keyval = it->first + "+" + it->second;
	// 	env[i] = new char[keyval.length()];
	// 	env[i] = const_cast<char *>(keyval.c_str());
	// 	i++;
	// }

	// for (size_t i = 0; i < requestEnv.size(); i++)
	// {
	// 	std::cout << env[i] << "\n";
	// }
}

