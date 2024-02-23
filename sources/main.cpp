/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:58:56 by shicham           #+#    #+#             */
/*   Updated: 2024/02/23 12:07:28 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/ConfigFile.hpp"
#include "../includes/Request.hpp"
#include "../includes/Client.hpp"

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
        Request               req;
        Client                client(req);

        MyConfigFile.parseConfigFile(configFile);
        req.SetConfigFile(MyConfigFile);
        client.serve();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
