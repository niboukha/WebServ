/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 10:27:55 by shicham           #+#    #+#             */
/*   Updated: 2024/02/22 06:59:19 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "WebServ.hpp"
#include "Server.hpp"
// #include "Request.hpp"
// class Request;

class ConfigFile
{
    private:
            std::vector<Server> servers;
    public:
        ConfigFile();
        ~ConfigFile();
        const std::vector<Server>& getServers() const;
        void   parseConfigFile(std::fstream &configFile);
        static  Server fillServer(std::fstream& configFile);
        static  mapStrVect  fillLocation(std::fstream& configFile);
        
};


#endif