/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 08:48:42 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/09 21:30:42 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../Utils/WebServ.hpp"
#include "../Request/Request.hpp"
#include "../Get/Get.hpp"
#include "../Delete/Delete.hpp"
#include "../Post/Post.hpp"

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
		std::string	headerRes;
		std::string	bodyRes;

		std::map<std::string, std::string>	mimeType;
		std::map<std::string, std::string>	extentionFile;

	public:

		Response( Request &request );
		~Response( );
		
		Stage				sendResponse(Stage &stage, std::string &reqBuff);

		void				setStatusCodeMsg(std::string& codeMsg);
		void				setPath(std::string pt);
		void				setHeaderRes(const std::string& header);

		const Request&		getRequest() const;
		const std::string&	getStatusCodeMsg() const;
		const std::string&	getPath() const;
		const std::string&	getHeaderRes() const;
		const std::string&	getBodyRes() const;

		std::string			getContentType( std::string &path );
		std::string			getContentLength( std::string &path );

		void				mapOfTypes( );
		void				UpdateStatusCode(std::string &s);
		void				isRealPath(std::string &path);
		void				throwNewPath(std::string msg, std::string code);

		std::string			concatenateIndexDirectory( );
		std::string			concatenatePath( std::string p );
		std::string			pathErrorPage(std::string code);
		std::string			getExtensionFile();


};

#endif
