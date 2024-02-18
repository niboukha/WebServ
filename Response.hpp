/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 08:48:42 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 12:46:45 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServ.hpp"
#include "Request.hpp"
#include "Get.hpp"
#include "Delete.hpp"

class Request;
class Get;
class Delete;

class Response
{
	private:

		Request		&req;

		Get			*get;
		Delete		*delt;
		// Post		*post;

		std::string					statusCodeMsg;
		std::string					path;


		std::map<std::string, std::string>	contType;

	public:

		Response( Request &request );
		~Response( );

		Stage	sendResponse(int stage);

		void	setStatusCodeMsg(std::string& statusCodeMsg);
		void	setPath(std::string& path);

		const Request&		getRequest() const;
		const std::string&	getStatusCodeMsg() const;
		const std::string&	getPath() const;

		std::string		getContentType( std::string &path );
		std::string		getContentLength( std::string &path );

		void			mapOfTypes( );
		std::string		concatenateIndexDirectory( std::string &file );
		std::string		concatenatePath( );
		std::string		pathErrorPage(std::string code);

		void			prefaceMethod( );

};

#endif
