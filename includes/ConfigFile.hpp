/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 10:27:55 by shicham           #+#    #+#             */
/*   Updated: 2024/02/23 12:03:38 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "WebServ.hpp"
#include "../includes/Server.hpp"
// #include "../includes/Request.hpp"
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
        static  void fillServer(std::fstream& configFile, Server& server);
        
        static  mapStrVect  fillLocation(std::fstream& configFile);
        static  void        addDirectivesMissingInLocation(mapStrVect &location);
        
};


#endif