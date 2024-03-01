/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:37:36 by shicham           #+#    #+#             */
/*   Updated: 2024/03/01 21:39:04 by shicham          ###   ########.fr       */
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
            ConfigFile          &configFile;
            // std::map<int, Client>   clients;
            std::vector<Request>    req;
            std::list<std::pair<int, Client> > clients;
            // std::vector<std::pair<int, Client> > clients;
        //     std::string              reqBuff;
        //     std::vector<std::string>    reqBuff;
    public:
        Multiplexer(ConfigFile& config);
        ~Multiplexer();

        const std::vector<int>&  getMasterSockets() const;
        const ConfigFile&  getConfigFile() const;

        void findClient(int fd, std::list<std::pair<int, Client> >::iterator& it);

        // void    setMasterSockets();
        // void    setConfigFile();

        void    createSockets();
        void    bindSocket(int fdServer, std::string add, std::string port);
        int    acceptNewConnection(int masterSocket);

        void    multiplexing();
        
        
        
};

#endif