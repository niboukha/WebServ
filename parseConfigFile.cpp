/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 18:57:46 by shicham           #+#    #+#             */
/*   Updated: 2024/02/08 09:57:06 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"


Location fillLocation(std::fstream& configFile)
{
    mapStrVect  locData;
    Location    location;
    std::string line;

    while (std::getline(configFile, line) 
        and line.substr(0, line.find_first_of(" ")).compare("location"))
    {
        
    
    }
    return location;
}
std::map<std::string, Location> fillLocations(std::fstream& configFile)
{
    std::map<std::string, Location> locations;
    std::string line;

    while (std::getline(configFile, line) 
        and line.substr(0, line.find_first_of(" ")).compare("server"))
    {
        
    
    }
    return locations;
    
}

Server  fillServer(std::fstream& configFile)
{
    std::string line, key;
    size_t       start, end;
    Server      server;
    mapStrVect  servData;
    std::map<std::string, Location> locations;
    std::vector<std::string>        values;

    while (std::getline(configFile, line))
    {
        if (line[0] == '\t' and isalpha(line[1]))
        {
            start = line.find_first_not_of("\t");
            end = line.find_first_of(" ", start);
            key = line.substr(start, start - end);
            values = StringOperations::parseString(line.substr(key.length(), line.length()));
            servData[key] = values;
        }
        else if (line[0] == '\t' and line[1] == '\t')
            locations = fillLocations(configFile);
        else
            throw SyntaxError();
    }
    server.setServerData(servData);
    server.setLocations(locations);
    return server;
}

std::vector<Server>   parseConfigFile(std::fstream& configFile)
{
    std::vector<Server> servers;
    Server              server;
    std::string         line;

    while (std::getline(configFile, line))
    {
        if (!line.compare("server"))
        {
            server = fillServer(configFile);
            servers.push_back(server);
        }
    }
    
    return servers;
}