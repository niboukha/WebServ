/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 10:30:06 by shicham           #+#    #+#             */
/*   Updated: 2024/03/18 00:51:23 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

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


void    ConfigFile::decodeLocationPrefix(std::string& location)
{
    char    decodeChar;

    for (size_t i = 0; i < location.size(); i++)
    {
        if((location[i] == '%' and i != location.length() - 2) and 
        ((isdigit(location[i + 1]) and isupper(location[i + 2]))
        or (isdigit(location[i + 1]) and isdigit(location[i + 2]))))
        {
            decodeChar = static_cast<char>(strtol(location.substr(i + 1, 2).c_str(), NULL, 16));
            location.replace(i, 3, 1, decodeChar);
            i += 3;
        }
    }
}

mapStrVect  ConfigFile::fillLocation(std::fstream& configFile)
{
    mapStrVect                                  location;
    std::string                                 key, line, trimLine;
    std::vector<std::string>                    values;
    std::vector<std::string>                    vect;
    mapStrVect::iterator                        cgi, upload, root;   

    while (std::getline(configFile, line))
    {
		if (line.find_first_not_of("\t") == 2 and std::isalnum(line[2]))//update
		{
            values = StringOperations::split(line ,"\t ");
            if (values.size() == 1)
                throw ("Config file : " +  values[0] + " empty directire");
            key = values[0];
			values.erase(values.begin());
            (location.find(key) != location.end()) ?
            throw DirectiveAlreadyExist() : false ;
			if (Server::locationValidDirective(key, values))
                location[key] = values;
            continue;
		}
        trimLine = StringOperations::trim(line);
        (trimLine.empty()) ? throw ("Config file: empty line in location block") : false;
        configFile.seekg(-(line.length() + 1), std::ios_base::cur);
        break;
    }
    
    cgi = location.find("cgi_pass");
    upload = location.find("upload_pass");
    root = location.find("root");
    vect.push_back("");
    
    (location.empty()) ?  throw("config file : location block empty") : false;

    (root == location.end() ) ? 
    throw ("config file  : invalid location block root directive required") : false;//update
    
    if(upload != location.end() \
    and cgi != location.end()) 
        upload->second = vect ;
    
    // if ( (upload != location.end() \
    // and cgi != location.end())): upload->second;//update
    // std::cout << "====> upload : " << upload->second.front() << ""
    
    addDirectivesMissingInLocation(location);//update
    
    return location;
}


void  ConfigFile::fillServer(std::fstream& configFile, Server& server)
{
    std::string                         line, trimLine;
    std::map<std::string, std::string>  servData;
    std::vector<std::string>            values;
    std::map<std::string, mapStrVect>   locations;
   
    
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
                    throw ("Config file : invalid location argument ") : false;
                (locations.find(values[1]) != locations.end()) ?
                   throw ("Config file : duplicate location block "): false;	//sould check if the location already exists
                decodeLocationPrefix(values[1]);
				locations[values[1]] = fillLocation(configFile);

            }
			else if (Server::serverValidDirective(values[0], values[1]))
			{
				if (!values[0].compare("error_page"))
					values.erase(values.begin());
                (servData.find(values[0]) != servData.end()) ? \
				throw ("Config file : duplicate directive " +  values[0] + " in server block ") : false;	
				servData[values[0]] = values[1];
			}
            continue;
		}
        trimLine = StringOperations::trim(line);
        (trimLine.empty()) ? throw ("Config file: empty line in server block") : false;
        configFile.seekg(-(line.length() + 1), std::ios_base::cur);
        break;
        
    }
    server.setServerData(servData);
    server.serverObligatoryDirectives();
    
    // (locations.find("/") == locations.end()) ? \
    // throw("Config file : default location required") : false;
    
    server.setLocations(locations);
}

void    ConfigFile::parseConfigFile(std::fstream &configFile)
{
    Server              server;
    std::string         line;

    configFile.seekg(0, std::ios::end);  
    if (configFile.tellg() == 0) 
        throw ("Config file is empty !!!");
    configFile.seekg(0, std::ios::beg); 
    
    while (std::getline(configFile, line))
    {
        line = StringOperations::trim(line);
        if (!line.compare("server"))
        {
            fillServer(configFile, server);
            const std::map<std::string, std::string>& mapDataServ = server.getServerData();
            std::string                                host, port, servName;
            
            host = mapDataServ.find("host")->second;
            port = mapDataServ.find("port")->second;
            (mapDataServ.find("server_name") != mapDataServ.end()) ? servName = mapDataServ.find("server_name")->second :
            servName = std::string("");
            for (size_t j = 0; j < servers.size(); j++)
            {
                const std::map<std::string, std::string>& mapDataS = servers[j].getServerData();
                
               if (!mapDataS.find("host")->second.compare(host) 
                    and !mapDataS.find("port")->second.compare(port)
                    and ((!servName.empty()  and mapDataS.find("server_name") != mapDataS.end()
                    and !mapDataS.find("server_name")->second.compare(servName)) 
                    or( servName.empty()  and mapDataS.find("server_name") == mapDataS.end())))
                    throw ("Config file : duplicate server block");
            }
            servers.push_back(server);
        }
        else if (line.empty())
            continue ;
        else
            throw SyntaxError();
    } 
    if (servers.empty())
        throw ("Config file is empty !!!");
}
 