/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:53:55 by shicham           #+#    #+#             */
/*   Updated: 2024/02/21 21:09:57 by shicham          ###   ########.fr       */
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
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "StringOperations.hpp"
#include <fcntl.h>
#include <stdio.h> 
#include <string.h> 

#define _XOPEN_SOURCE 700
#define	CARIAGE_RETURN "\n\r"
#define PORT 7000

enum    Stage
{
    REQLINE,
    REQHEADER,
    REQBODY,
    RESHEADER,
    RESBODY,
    RESEND
};

typedef std::map<std::string, std::vector<std::string> > mapStrVect;//stringToVectMap
typedef std::map<std::string, std::vector<std::string> >::value_type MapValueType;

// #include "Server.hpp"

#endif