/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:15:01 by shicham           #+#    #+#             */
/*   Updated: 2024/03/02 06:32:32 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP 
#define REQUEST_HPP

#include "WebServ.hpp"
#include "../includes/ConfigFile.hpp"

class ConfigFile;

class Request
{
    private:
		ConfigFile							configFileData;
		mapStrVect							location;
		std::map<std::string, std::string>	server;
		std::map<std::string, std::string>	headers;
		std::map<std::string, std::string>	errorPages;
		std::string							method;
		std::string							requestedPath;
		std::string							protocolVersion;
		std::string							uri;
		std::string							autority;
		std::string							scheme;
		std::string							queryParameters;
		
    public:
        Request();
		Request(ConfigFile& config);
        ~Request();
		const	Request&	operator=(const Request& copy);
        void   parseRequest(std::string &buff, Stage &stage);
		
		const	std::string getMethod( ) const;
		const	mapStrVect  getLocation() const;
		const	std::string getRequestedPath( ) const;
		const	std::string getProtocolVersion( ) const;
		const	std::map<std::string, std::string>	getServer( ) const;
		const	std::map<std::string, std::string>	getHeaders() const;
		const	std::map<std::string, std::string>	getErrorPages() const;

		void		fillErrorPages();
		void		parseUri( );
		void		decodeUri();
		std::string decodePercentEncoded(std::string hexastr);
		void		parseRequestLine(std::string    &buff);
		void		parseHeader(std::string& buff);
        void		SetConfigFile(ConfigFile& configFile);
		void		matchingLocation();
		Server		matchingServer();
		size_t		longestMatchingLocation(const std::string& prefix);

		class BadRequest : std::exception
		{
			private:
					std::pair<std::string, std::string>	pairCodePath;
			public:
				BadRequest(const char* codeError, const char* path) : 
					pairCodePath(codeError, path)
				{
				}
				virtual ~BadRequest() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
				{
				}
				virtual const char* what() const throw() 
				{
					return NULL;
    			}
				const std::pair<std::string, std::string>& getPairCodePath() const
				{
					return pairCodePath;
				}
		};

		class NotImplemented : std::exception
		{
			private:
					std::pair<std::string, std::string>	pairCodePath;
			public:
				NotImplemented(const char* codeError, const char* path) : 
					pairCodePath(codeError, path)
				{
				}
				virtual ~NotImplemented() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
				{
				}
				virtual const char* what() const throw() 
				{
					return NULL;
    			}
				const std::pair<std::string, std::string>& getPairCodePath() const
				{
					return pairCodePath;
				}
		};

		class NotSupported : std::exception
		{
			private:
					std::pair<std::string, std::string>	pairCodePath;
			public:
				NotSupported(const char* codeError, const char* path) : 
					pairCodePath(codeError, path)
				{
				}
				virtual ~NotSupported() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
				{
				}
				virtual const char* what() const throw() 
				{
					return NULL;
    			}
				const std::pair<std::string, std::string>& getPairCodePath() const
				{
					return pairCodePath;
				}
		};
		
};


#endif