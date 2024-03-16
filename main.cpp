/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:58:56 by shicham           #+#    #+#             */
/*   Updated: 2024/03/16 12:23:01 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "ConfigFile/ConfigFile.hpp"
#include "Request/Request.hpp"
#include "Client/Client.hpp"
#include "Multiplexer/Multiplexer.hpp"

int main(int ac, char **av)
{
    std::fstream           configFile;
    ConfigFile             MyConfigFile;
    Multiplexer            multiplexer;
        
    try
    {
        if (ac == 2)
        {
            configFile.open(av[1]);
            (!configFile.is_open()) ?
                throw FailedToOpenFile() : false;
        }
        else
        {
            configFile.open("webserv.config");
            (!configFile.is_open()) ?
                throw FailedToOpenFile() : false;
        }
        MyConfigFile.parseConfigFile(configFile);
        configFile.close();
        multiplexer.setServers(MyConfigFile.getServers());
        multiplexer.multiplexing();
        
    }
    catch(const std::exception& e)
    {
        // std::cout << "iiii-> iii\n";
        std::cerr << e.what() << '\n';
        configFile.close();
    }
    catch(const char *msg)
    {
        std::cerr << msg << '\n';
        configFile.close();
    }
    catch(std::string& msg)
    {
        std::cerr << msg << '\n';
        configFile.close();
    }
    return (0);
}
