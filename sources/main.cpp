/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:58:56 by shicham           #+#    #+#             */
/*   Updated: 2024/03/04 15:35:02 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ConfigFile.hpp"
#include "../includes/Request.hpp"
#include "../includes/Client.hpp"
#include "../includes/Multiplexer.hpp"

int main(int ac, char **av, char** env)
{
    std::fstream configFile;
    ConfigFile            MyConfigFile;
    Multiplexer            multiplexer;
        
    try
    {
        (void)env;
        if (ac != 2)
            throw InvalidArguments();
        configFile.open(av[1]);
        if (!configFile.is_open())
            throw FailedToOpenFile();
        MyConfigFile.parseConfigFile(configFile);
        configFile.close();
        multiplexer.setServers(MyConfigFile.getServers());
        multiplexer.multiplexing();
        
    }
    catch(const std::exception& e)
    {
        std::cout << "iiii-> iii\n";
        std::cerr << e.what() << '\n';
        configFile.close();
    }
    catch(const char *msg)
    {
        std::cerr << msg << '\n';
        configFile.close();
    }
    return (0);
}
