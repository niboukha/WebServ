/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 08:48:42 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/22 21:55:50 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServ.hpp"
#include "Request.hpp"
#include "Get.hpp"
#include "Delete.hpp"
#include "Post.hpp"

class Request;
class Get;
class Delete;
class Post;

class Response
{
	private:

		Request	&req;

		Get		*get;
		Post	*post;
		Delete	*delt;

		std::string	statusCodeMsg;
		std::string	path;
		std::string	body;

		std::string	headerRes;
		std::string	bodyRes;
		
		int			fd;

		std::map<std::string, std::string>	mimeType;

	public:

		Response( Request &request );
		~Response( );

		Stage	sendResponse(Stage &stage);

		void	setStatusCodeMsg(std::string& codeMsg);
		void	setPath(std::string pt);
		void	setBody(const std::string& bdy);
		void	setBodyRes(const std::string& bdy);
		
		const Request&		getRequest() const;
		const std::string&	getStatusCodeMsg() const;
		const std::string&	getPath() const;
		const std::string&	getBody() const;
		const int&			getfd() const;
		const std::string&	getHeaderRes() const;
		const std::string&	getBodyRes() const;
		
		

		std::string		getContentType( std::string &path );
		std::string		getContentLength( std::string &path );

		void			mapOfTypes( );
		std::string		concatenateIndexDirectory( std::string &file );
		std::string		concatenatePath( );
		std::string		pathErrorPage(std::string code);

		void			throwNewPath(std::string msg, std::string code);

		void			prefaceMethod( );

};

#endif
