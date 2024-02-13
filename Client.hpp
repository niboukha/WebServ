/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:13:12 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/13 15:18:18 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "WebServ.hpp"

class   Request;

class   Client
{
	private:
	
		Request		&req;
		std::string	recievedReq;

	public:

		Client();
		~Client();

		const std::string&	getRecievedReq() const;
        
		void	recieveRequest();
		void	recieveSendResponse();
        
}

#endif
