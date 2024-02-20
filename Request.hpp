/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:37:55 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 13:56:26 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "WebServ.hpp"
#include "Server.hpp"

class Request
{
	private :

		std::map<std::string, std::string>	server;
		
		mapStrVect	location;
		mapStrVect	headers;

		std::string	method;
		std::string	requestedPath;
		std::string	protocolVersion;

	public :

		Request ();
		~Request ();

		Stage	parseRequest(std::string buffer);

		const mapStrVect	getLocation() const;
		const mapStrVect	getHeaders() const;
		const std::string	getRequestedPath() const;
		const std::string	getMethod() const;
		const std::string	getProtocolVersion() const;

		const std::map<std::string, std::string>&	getServer() const;
};

#endif
