/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:37:36 by shicham           #+#    #+#             */
/*   Updated: 2024/03/04 17:54:27 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include "WebServ.hpp"
#include "ConfigFile.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Multiplexer.hpp"
#include "Request.hpp"

class ConfigFile;
class Server;
class Client;

class Multiplexer
{
    private:
            std::vector<int>    masterSockets;
            std::vector<Server>   servers;

            std::vector<Request>    req;
            std::list<Client>       clients;
            
    public:
        Multiplexer();
        ~Multiplexer();

        void setServers(const std::vector<Server>& servers);
        void    multiplexing();
        
        
        
};

#endif