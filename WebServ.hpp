/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:53:55 by shicham           #+#    #+#             */
/*   Updated: 2024/02/21 13:42:23 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <fstream>
#include <vector>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sstream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib> 
#include <iostream> 
#include <time.h> 

#define _XOPEN_SOURCE 700
#define	CRLF "\r\n" //carriage return and line feed
#define	PATH_MIME "/nfs/homes/niboukha/Desktop/webServ/mimetype.txt"
typedef std::map<std::string, std::vector<std::string> > mapStrVect;

enum	Stage
{
	REQLINE,
	REQHEADER,
	REQBODY,
	RESHEADER,
	RESBODY,
	RESEND
};

// #include "Request.hpp"
// #include "Response.hpp"
// #include "Client.hpp"
// #include "Get.hpp"
// #include "Post.hpp"
// #include "Delete.hpp"

#include "Exceptions.hpp"
#include "Utils.hpp"

class	Server;
class	StringOperations;

std::vector<Server>  parseConfigFile(std::fstream& configFile);

#endif
