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
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"

class Response
{
	private:
		
		char		*localCheck;

		Request		&req;

		Get			*get;
		Post		*post;
		Delete		*delt;
		
	public:

		Response(Request &request);
		~Response();
		
		const Request	getRequest() const;

		
		void	fillResponse();
};

#endif
