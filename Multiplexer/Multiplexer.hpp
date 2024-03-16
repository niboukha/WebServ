/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:37:36 by shicham           #+#    #+#             */
/*   Updated: 2024/03/16 11:05:19 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include "../Utils/WebServ.hpp"
#include "../ConfigFile/ConfigFile.hpp" 
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
// #include "Multiplexer.hpp"
#include "../Request/Request.hpp"

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

        void    readReq(Client& cl);
        void    sendRes(Client& cl);
        void    clear(fd_set& tmpR, fd_set& tmpW, Client& client);
        void    reqServers(Server& serv, std::vector<Server>& servs);
        void    dropClient(Client& cl);
        
        
        
};

#endif
