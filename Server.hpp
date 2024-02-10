/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:26 by shicham           #+#    #+#             */
/*   Updated: 2024/02/09 09:22:18 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServ.hpp"

class   Location;
typedef std::map<std::string, std::vector<std::string> > mapStrVect;

class Server
{
    private:
        mapStrVect                      serverData;
        std::map<std::string, Location> locations;
        Location *loc;
    public:
        Server();
        ~Server();
        
        void    setServerData(mapStrVect& servData);
        void    setLocations(std::map<std::string,  Location>& locs);
        
        const mapStrVect&                       getServerData() const;
        const std::map<std::string,  Location>& getLocations() const;
        
        static  bool    serverValidDirective(std::string directive);
        static  bool    isValidHostValue(std::string hostValues);
        static  bool    isValidPortValue(std::string portValue);


        
};

#endif