/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:13:12 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/21 18:12:50 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServ.hpp"
// #include "Response.hpp"
#include "Request.hpp"

// class   Response;
class	Request;

class   Client
{
	private:

		// Response	res;
		Request		&req;
		
		std::string	recievedReq;
        Stage        stage;
		std::string	reqBuff;	

		void	recieveRequest();
		void	sendResponse();

	public:

		Client();
		~Client();
		Client(Request& request);

		const std::string&	getRecievedReq() const;
		const Stage&		getStage() const;

		void	serve();

};

#endif
