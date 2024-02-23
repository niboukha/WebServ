/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:15:01 by shicham           #+#    #+#             */
/*   Updated: 2024/02/22 10:03:46 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP 
#define REQUEST_HPP

#include "WebServ.hpp"
#include "ConfigFile.hpp"

class ConfigFile;

class Request
{
    private:
		ConfigFile							configFileData;
		std::map<std::string, std::string>	server;
		mapStrVect							location;
		std::map<std::string, std::string>	headers;
		std::string							method;
		std::string							requestedPath;
		std::string							protocolVersion;
		std::string							uri;
		std::string							autority;
		std::string							scheme;
		std::string							queryParameters;
		
    public:
        Request();
        ~Request();
        void   parseRequest(std::string &buff, Stage &stage);
		
		const	std::string getMethod( ) const;
		const	mapStrVect  getLocation() const;
		const	std::string getRequestedPath( ) const;
		const	std::string getProtocolVersion( ) const;
		const	std::map<std::string, std::string>	getServer( ) const;
		const	std::map<std::string, std::string>	getHeaders() const;

		
		void		parseUri( );
		void		decodeUri();
		std::string decodePercentEncoded(std::string hexastr);
		void		parseRequestLine(std::string    &buff);
		void		parseHeader(std::string& buff);
        void		SetConfigFile(ConfigFile& configFile);
		void		matchingLocation();
		Server		findServer();
		size_t		longestMatchingLocation(const std::string& prefix);
};


#endif