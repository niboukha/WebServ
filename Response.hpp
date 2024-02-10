/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 08:48:42 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/10 10:44:02 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServ.hpp"
#include "Request.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"

class Request;

class Response
{
	private:

		std::map<std::string, std::string>	type;
		// char								*localCheck;

		Request		&req;
		Get			*get;
		// Post		*post;
		// Delete		*delt;


	public:

		Response(Request &request);
		~Response();


		const Request								getRequest() const;
		const std::map<std::string, std::string>	getType();

		void			fillResponse();
		void			contentType(std::map<std::string, std::string>	&tP);
		long int		contentLength(std::string path);
		void			trimString( std::string &s );

		std::vector<std::string>	moreThanKey(std::string s);

};

#endif
