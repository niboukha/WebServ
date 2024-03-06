/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 10:30:06 by shicham           #+#    #+#             */
/*   Updated: 2024/03/06 21:25:19 by shicham          ###   ########.fr       */
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
    std::vector<std::string>    vect;
    
    vect.push_back("");
    // if (location.find("root") == location.end())//redo lbal !!!
    //     location["root"] = vect;
    if (location.find("index") == location.end())
        location["index"] = vect;
    if (location.find("autoindex") == location.end())
        location["autoindex"] = vect;
    if (location.find("allow_methodes") == location.end())
        location["allow_methodes"] = vect;
    if (location.find("upload_pass") == location.end())
        location["upload_pass"] = vect;
    if (location.find("cgi_pass") == location.end())
        location["cgi_pass"] = vect;
    if (location.find("return") == location.end())//update!!!
        location["return"] = vect;
}

mapStrVect  ConfigFile::fillLocation(std::fstream& configFile)
{
    mapStrVect location;
    std::string          key, line;
    std::vector<std::string>          values;

    while (std::getline(configFile, line))
    {
		if (line.find_first_not_of("\t") == 2 and std::isalnum(line[2]))//update
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
            (location.find("root") == location.end()) ? 
            throw ("config file error : invalid location block") : false;//update
            addDirectivesMissingInLocation(location);//update
            configFile.seekg(-(line.length() + 1), std::ios_base::cur);
            break;
        }
    }
    return location;
}

void  ConfigFile::fillServer(std::fstream& configFile, Server& server)
{
    std::string                         line;
    std::map<std::string, std::string>  servData;
    std::vector<std::string>            values;
    std::map<std::string, mapStrVect>   locations;
    std::string                          trimLine;
    
    while (std::getline(configFile, line))
    {
		if (!line.find("\t") and std::isalnum(line[1]))//update
		{
			values = StringOperations::split(line, " \t");
			if ((values[0].compare("error_page") and values.size() != 2) 
            or (!values[0].compare("error_page") and values.size() != 3))
				throw InvalidNumberOfArguments();
			if (!values[0].compare("location"))
            {
                (values[1].find("/") or values[1].find_last_of("/") != values[1].length() - 1) ?
                    throw ("Config file error: invalid location argument ") : false;
                (locations.find(values[1]) != locations.end()) ?
                   throw ("Config file : duplicate location block "): false;	//sould check if the location already exists
				locations[values[1]] = fillLocation(configFile);
            }
			else if (Server::serverValidDirective(values[0], values[1]))
			{
				if (!values[0].compare("error_page"))
					values.erase(values.begin());
                (servData.find(values[0]) != servData.end()) ? \
				throw ("Config file : duplicate directive " +  values[0] + " in server") : false;	
				servData[values[0]] = values[1];
			}
		}
		else 
		{
            trimLine = StringOperations::trim(line);
            (trimLine.empty()) ? throw SyntaxError() : false;
			configFile.seekg(-(line.length() + 1), std::ios_base::cur);
            break;
		}
        
    }
    server.setServerData(servData);
    server.serverObligatoryDirectives();
    (locations.find("/") == locations.end()) ? \
    throw("Config file : default location doesn't exist") : false;
    server.setLocations(locations);
}

void    ConfigFile::parseConfigFile(std::fstream &configFile)
{
    Server              server;
    std::string         line;

    while (std::getline(configFile, line))
    {
        line = StringOperations::trim(line);
        if (!line.compare("server"))
        {
            fillServer(configFile, server);
            servers.push_back(server);
        }
        else if (line.empty())//to check 
            continue ; 
        else
            throw SyntaxError();
    }
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