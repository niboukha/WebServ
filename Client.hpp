/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:13:12 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/22 19:48:34 by niboukha         ###   ########.fr       */
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

		Request		&req;
		Response	res;

		std::string	recievedReq;
		Stage		stage;
		std::string	reqBuff;

		int 	sockFd;
		int		newSockFd;

		void	recieveRequest();
		void	sendResponse();

	public:

		Client(Request& request);

		~Client();

		const std::string&	getRecievedReq() const;
		const Stage&		getStage() const;

		void	serve();

};

#endif
