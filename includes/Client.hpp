/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:13:12 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/01 13:59:04 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServ.hpp"
#include "../includes/Response.hpp"
#include "../includes/Request.hpp"

class	Response;
class	Request;

class	Client
{
	private:

		Request		&req;
		Response	res;

		Stage		stage;
		std::string	reqBuff;
		std::string	sendBuff;
		void	recieveRequest();
		void	sendResponse();

	public:

		Client(Request& request);
		// Client(const Client&  copy) : req(copy.req) , res(copy.req)
		// {
		// 	*this = copy;
		// }
		// Client() : req(){};

		~Client();


		void	serve();
		
		const Stage&		getStage() const;

		// const	Client& operator=(const Client& other);
		

		const std::string&	getReqBuff() const;
		const std::string&	getSendBuff() const;

		void	setReqBuff(const std::string& buff);
		void	setSendBuff(const std::string& buff);


};

#endif
