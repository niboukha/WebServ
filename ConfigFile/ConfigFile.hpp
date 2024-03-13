/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 10:27:55 by shicham           #+#    #+#             */
/*   Updated: 2024/03/13 14:04:29 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "../Utils/WebServ.hpp"
#include "../Server/Server.hpp"

class ConfigFile
{
    private:
        std::vector<Server> servers;
        static void   decodeLocationPrefix(std::string& location);
        
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