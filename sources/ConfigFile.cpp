/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 10:30:06 by shicham           #+#    #+#             */
/*   Updated: 2024/02/23 12:07:48 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigFile.hpp"

ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

const std::vector<Server>& ConfigFile::getServers() const
{
    return servers;
}

void    ConfigFile::addDirectivesMissingInLocation(mapStrVect &location)//update
{
    if (location.find("root") == location.end())//redo lbal !!!
        location["root"].push_back("");
    if (location.find("index") == location.end())
        location["index"].push_back("");
    if (location.find("autoindex") == location.end())
        location["autoindex"].push_back("");
    if (location.find("allow_methodes") == location.end())
        location["allow_methodes"].push_back("");
    if (location.find("upload_pass") == location.end())
        location["upload_pass"].push_back("");
    
}
mapStrVect  ConfigFile::fillLocation(std::fstream& configFile)
{
    mapStrVect location;
    std::string          key, line;
    std::vector<std::string>          values;

    while (std::getline(configFile, line))
    {
		if (line.find_first_not_of("\t") == 2 and isalpha(line[2]))
		{
            values = StringOperations::split(line ,"\t ");
            key = values[0];
			values.erase(values.begin());
			if (Server::locationValidDirective(key, values))
            {
                (location.find(key) != location.end()) ? 
				throw DirectiveAlreadyExist() : false;
                location[key] = values;
            }
		}
		else
        {
            addDirectivesMissingInLocation(location);//update
            configFile.seekg(-(line.length() + 1), std::ios_base::cur);
            break;
        }
    }
    return location;
}

Server  ConfigFile::fillServer(std::fstream& configFile)
{
    std::string line, key;
    Server      server;
    std::map<std::string, std::string>  servData;
    std::vector<std::string>        values;
    std::map<std::string, mapStrVect>   locations;
    
    while (std::getline(configFile, line))
    {
		if (!line.find("\t") and isalpha(line[1]))
		{
			values = StringOperations::split(line, " \t");
			key = values[0];
			if ((values[0].compare("error_page") and values.size() != 2) 
            or (!values[0].compare("error_page") and values.size() != 3))
				throw InvalidNumberOfArguments();
			if (!values[0].compare("location"))
				locations[values[1]] = fillLocation(configFile);//sould check if the location already exists
			else if (Server::serverValidDirective(values[0], values[1]))
			{
				if (!values[0].compare("error_page"))
					values.erase(values.begin());
                (servData.find(values[0]) != servData.end()) ? \
				throw DirectiveAlreadyExist(): false;	
				servData[values[0]] = values[1] ;
			}
		}
		else 
		{
			configFile.seekg(-(line.length() + 1), std::ios_base::cur);
            break;
		}
    }
    server.setServerData(servData);
    server.addErrorPagesMissing();
    server.serverObligatoryDirectives();
    server.setLocations(locations);
    return server;
}

 void    ConfigFile::parseConfigFile(std::fstream &configFile)
 {
    std::vector<Server> servers;
    Server              server;
    std::string         line;

    while (std::getline(configFile, line))
    {
        line = StringOperations::trim(line);
        if (!line.compare("server"))
        {
            server = fillServer(configFile);
            servers.push_back(server);
        }
        else if (line.empty())
			continue ; 
        else
            throw SyntaxError();   
    }
	this->servers = servers;
    // std::cout << "***** vectors of servers : ***** " << std::endl;
    // for (size_t i = 0; i < servers.size(); i++)
    // {
    //     std::cout << "  ---> every server  data : " << std::endl;
    //     for (std::map<std::string, std::string>::const_iterator  it = servers[i].getServerData().begin(); 
    //     it != servers[i].getServerData().end(); it++)
    //     {
    //         std::cout << "  key : " << it->first << "|  value : " \
    //         << it->second << std::endl;
    //     }
    //     std::cout << "	===> locations: " << std::endl;
    //     for (std::map<std::string, mapStrVect>::const_iterator 
    //     it = servers[i].getLocations().begin() ; 
    //     it !=  servers[i].getLocations().end(); it++)
    //     {
    //         std::cout << "  -->location key's : " << it->first << std::endl;
    //         for (mapStrVect::const_iterator i = it->second.begin(); i != it->second.end(); i++)
    //         {
    //            std::cout << "		==> key : " << i->first << " | values : ";
	// 		   for (size_t k = 0; k < i->second.size(); k++)
	// 		   {
	// 				std::cout << i->second[k] << " "; 
	// 		   }
	// 		   std::cout << std::endl;
    //         }
            
    //     }
        
    // }
    
 }