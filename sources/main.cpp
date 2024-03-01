/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:58:56 by shicham           #+#    #+#             */
/*   Updated: 2024/03/01 18:03:37 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ConfigFile.hpp"
#include "../includes/Request.hpp"
#include "../includes/Client.hpp"
#include "../includes/Multiplexer.hpp"

int main(int ac, char **av, char** env)
{
    try
    {
        (void)env;
        if (ac != 2)
            throw InvalidArguments();
        std::fstream                        configFile(av[1]);
       
        if (!configFile.is_open())
            throw FailedToOpenFile();
        ConfigFile            MyConfigFile;
        // Request               req;
        // Client                client(req);
        Multiplexer            multiplexer(MyConfigFile);

        MyConfigFile.parseConfigFile(configFile);
        configFile.close();
        multiplexer.multiplexing();
        // req.SetConfigFile(MyConfigFile);
        // client.serve();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const char *msg)
    {
        std::cerr << msg << '\n';
    }
}
