/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 11:06:51 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/12 12:15:01 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include "Server.hpp"

mapStrVect  fillServerData()
{
    mapStrVect  ser;

    ser["host"].push_back("10.11.3.2");
    ser["port"].push_back("80");
    ser["server_name"].push_back("www.WebServ.com");
    ser["error_page"].push_back("404 /error_pages/404.html");
    ser["error_page"].push_back("501 /error_pages/501.html");
    ser["client_max_body_size"].push_back("10");
    return (ser);
}

// std::map<std::string, >  fillLocationClass()
// {
//     std::map<std::string, mapStrVect > loc;
//     mapStrVect  setLoc;
//     mapStrVect    loca;
//     Server        s;
    
//     setLoc["root"].push_back("/requests_files");
//     setLoc["allow"].push_back("GET");
//     setLoc["allow"].push_back("POST");
//     setLoc["allow"].push_back("DELETE");
//     setLoc["autoindex"].push_back("on");
//     setLoc["index"].push_back("index.html");
    
//     loca.setLocationData(setLoc);
//     loca.getLocationData();
//     loc["/"] = loca;
//     s.setLocations(loc);
//     return (s.getLocations());
// }

int main(int ac, char **av)
{
    (void)av;
    try
    {
        if (ac != 1)
            throw InvalidArguments();
        
        Server  obj;
        mapStrVect  ser;

        ser = fillServerData();
        obj.setServerData(ser);
        
        mapStrVect::iterator it = ser.begin();
        for (; it != ser.end(); it++)
        {
            std::cout << it->first << '\t';
            for (size_t i = 0; i < it->second.size(); i++)
            {
                std::cout << it->second[i] << " ";
            }
            std::cout << "\n";
        }
        
        // std::map<std::string,  Location>::iterator itr = loc.begin();
        
        // for (; itr != loc.end(); itr++)
        // {
        //     std::cout << itr->first << '\n';
        //     mapStrVect l = itr->second.getLocationData();
        //     std::cout << l.size();
        //     mapStrVect::iterator it = l.begin();
        //     for (; it != l.end(); it++)
        //     {
        //         std::cout << it->first << "\t";
        //         for (size_t i = 0; i < it->second.size(); i++)
        //         {
        //             std::cout << it->second[i] << " ";
        //         }
        //         std::cout << "\n";
        //     }
        // }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}