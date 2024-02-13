/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/13 15:07:17 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	req = new Request(*this);
}

Client::~Client()
{

}

const std::string&	Client::getRecievedReq() const
{
	return (recievedReq);
}


