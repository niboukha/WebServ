/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:26 by shicham           #+#    #+#             */
/*   Updated: 2024/02/12 12:14:56 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServ.hpp"

typedef std::map<std::string, std::vector<std::string> > mapStrVect;

class Server
{
    private:
        mapStrVect                      serverData;
    //    std::map<std::string, mapStrVect >                     serverData;
        
    public:
        Server();
        ~Server();
        
        void    setServerData(mapStrVect& servData);
        
        const mapStrVect&                       getServerData() const;
        

        static  bool    serverValidDirective(std::string directive);
        static  bool    isValidHostValue(std::string hostValues);
        static  bool    isValidPortValue(std::string portValue);


        
};

#endif