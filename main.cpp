/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:58:56 by shicham           #+#    #+#             */
/*   Updated: 2024/02/07 18:55:41 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

int main(int ac, char **av)
{
    try
    {
        if (ac != 2)
            throw InvalidArguments();
        std::fstream                        configFile(av[1]);
       
        if (!configFile.is_open())
            throw FailedToOpenFile();
            
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}