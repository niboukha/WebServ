/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:13:12 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/12 09:38:02 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../Utils/WebServ.hpp"
#include "../Response/Response.hpp"
#include "../Request/Request.hpp"

class	Response;
class	Request;

class	Client
{
	private:

		Request		req;
		Response	res;

		Stage		stage;
		std::string	reqBuff;
		std::string	sendBuff;

		int			fd;
		
		void		recieveRequest();
		void		sendResponse();

	public:

		Client(std::vector<Server>& servers, int &fd);
		Client(const Client&copy);
	
		~Client();

		// const Client& operator=(const Client& copy);
		void	serve();
	
		const int&	getFd() const;
		void  setFd(int& fd);
		
		const Stage&		getStage() const;
		const std::string&	getReqBuff() const;
		const std::string&	getSendBuff() const;

		void				setReqBuff(const std::string& buff);
		void				setSendBuff(const std::string& buff);

		void				setStage(const Stage& st);

};

#endif
