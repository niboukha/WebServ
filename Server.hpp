/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:35:26 by shicham           #+#    #+#             */
/*   Updated: 2024/02/22 06:39:50 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServ.hpp"

class      Location;

class Server
{
    private:
        std::map<std::string, std::string> serverData;//to check
        std::map<std::string,  mapStrVect> locations;
    public:
        Server();
        ~Server();
        void    setServerData(std::map<std::string, std::string>& servData);
        void    setLocations(std::map<std::string,  mapStrVect>& locs);
        const   std::map<std::string, std::string>& getServerData() const;
        const   std::map<std::string,  mapStrVect>& getLocations() const;
        bool  serverObligatoryDirectives();
        // void     addErrorPagesMissing();
        
        static  bool serverValidDirective(std::string &directive, std::string& value);
        static  bool  isValidHost(std::string &hostValue);
        static  bool  isValidPort(std::string &portValue);
        static  bool  isValidErrorPage(std::string &errorPageValue);
        static  bool  isValidClientMaxBodySize(std::string &ClientMaxBodySizeValue);
        static  bool  isValidServerName(std::string &serverNameValue);
        
        static  bool  locationValidDirective(std::string &directive, std::vector<std::string> &values);
        static  bool  isValidRoot(std::vector<std::string> &rootValue);
        static  bool  isValidAllowMethodes(std::vector<std::string> &allowMethodesValue);
        static  bool  isValidIndex(std::vector<std::string> &indexValue);
        static  bool  isValidAutoIndex(std::vector<std::string> &autoindexValue);
        static  bool  isValidUploadPass(std::vector<std::string> &uploadValue);
        
        
};

#endif