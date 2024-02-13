/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 08:48:42 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/13 13:56:56 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServ.hpp"                                                                                                                                         

class Request;
class Get;

class Response
{
	private:

		std::map<std::string, std::string>	contType;

		Request		&req;
		Get			*get;
		// char		*localCheck;
		// Post		*post;
		// Delete	*delt;

	public:

		Response( Request &request );
		~Response();

		const Request	getRequest() const;
		std::string		getContentType( std::string &path );
		std::string		getContentLength( std::string &path );

		
		void			mapOfTypes( );
		std::string		concatenateIndexDirectory( std::string &file );
		std::string		concatenatePath( );

		void			prefaceMethod( );

};

#endif
