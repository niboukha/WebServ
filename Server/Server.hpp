/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:26 by shicham           #+#    #+#             */
/*   Updated: 2024/03/15 16:16:21 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Utils/WebServ.hpp"

class Server
{
    private:
        std::map<std::string, std::string> serverData;
        std::map<std::string,  mapStrVect> locations;

        int                 masterSocket;
        
        std::string         clientIp;
        
        static bool     (*functionsLocation[])(std::vector<std::string> &);
    public:
        Server();
        ~Server();

        const int&  getMasterSocket() const;
        const std::string& getClientIp() const;
        void    setServerData(std::map<std::string, std::string>& servData);
        void    setLocations(std::map<std::string,  mapStrVect>& locs);
        const   std::map<std::string, std::string>& getServerData() const;
        const   std::map<std::string,  mapStrVect>& getLocations() const;

        void    createAndBindSocket(fd_set& readFds);
        int    acceptNewConnection(fd_set& readFds, fd_set& writeFds);
        
        bool    serverObligatoryDirectives();
        static  bool  serverValidDirective(std::string &directive, std::string& value);//update
        static  bool  isValidClientMaxBodySize(std::string &ClientMaxBodySizeValue);
        
        static  bool  locationValidDirective(std::string &directive, std::vector<std::string> &values);
        static  bool  isValidRoot(std::vector<std::string> &rootValue);

        static  bool  isValidAutoIndex(std::vector<std::string> &autoindexValue);
        static  bool  isValidUploadPass(std::vector<std::string> &uploadValue);
        static  bool  isValidCgiPass(std::vector<std::string> &cgiValue);
        static  bool  isValidReturn(std::vector<std::string> &returnValue);
        
        
};

#endif