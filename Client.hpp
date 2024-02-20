/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:13:12 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 13:51:11 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServ.hpp"
#include "Response.hpp"
#include "Request.hpp"

class	Response;
class	Request;

class	Client
{
	private:

		Response	res;
		Request		req;

		std::string	recievedReq;
		Stage		stage;

		void	recieveRequest(std::string buffer);
		void	sendResponse();

	public:

		Client();
		~Client();

		const std::string&	getRecievedReq() const;
		const Stage&		getStage() const;

		void	serve();

};

#endif
