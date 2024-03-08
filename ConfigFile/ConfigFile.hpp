/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 10:27:55 by shicham           #+#    #+#             */
/*   Updated: 2024/03/08 09:57:24 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "../Utils/WebServ.hpp"
#include "../Server/Server.hpp"
// #include "../Request/Request.hpp"
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