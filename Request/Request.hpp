/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:15:01 by shicham           #+#    #+#             */
/*   Updated: 2024/03/15 16:17:19 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP 
#define REQUEST_HPP

#include "../Utils/WebServ.hpp"
#include "../ConfigFile/ConfigFile.hpp"

class ConfigFile;

class Request
{
    private:
		std::vector<Server>					servs;
		mapStrVect							location;
		std::map<std::string, std::string>	server;
		std::map<std::string, std::string>	headers;
		std::map<std::string, std::string>	errorPages;
		std::string							method;
		std::string							requestedPath;
		std::string							protocolVersion;
		std::string							uri;
		std::string							queryParameters;
		std::string							clientIp;
		
		void		decodeUri();
		void		validateRequestHeader();
		void		fillErrorPages();
		Server&		matchingServer();
		size_t		longestMatchingLocation(const std::string& prefix);
		
    public:
        Request();
		Request(std::vector<Server>& servers);
        ~Request();
		
        void   parseRequest(std::string &buff, Stage &stage);
		
		const		std::string getMethod( ) const;
		const		mapStrVect  getLocation() const;
		const		std::string getRequestedPath( ) const;
		const		std::string getProtocolVersion( ) const;
		const		std::string getQueryParameters( ) const;
		const		std::map<std::string, std::string>	getServer( ) const;
		const		std::map<std::string, std::string>	getHeaders() const;
		const		std::map<std::string, std::string>	getErrorPages() const;

		void		setMethod( std::string method);
		void		setProtocolVersion( std::string httpVersion);

		void		parseUri( );
		void		parseRequestLine(std::string    &buff);
		void		parseHeader(std::string& buff, size_t& found);
     
		void		matchingLocation();

		void   		fillLocation();

		
};


#endif