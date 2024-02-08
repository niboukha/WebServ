/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:53:55 by shicham           #+#    #+#             */
/*   Updated: 2024/02/08 09:43:42 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <map>
#include <exception>
#include "Exceptions.hpp"
#include <fstream>
#include <vector>
#include "StringOperations.hpp"

typedef std::map<std::string, std::vector<std::string> > mapStrVect;

#include "Server.hpp"
#include "Location.hpp"

class Server;
class  Location;
class StringOperations;

std::vector<Server>  parseConfigFile(std::fstream& configFile);

#endif