/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:53:55 by shicham           #+#    #+#             */
/*   Updated: 2024/02/27 09:26:42 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <string>
#include <map>
#include <exception>
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
#include <exception>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <iterator>
#include <arpa/inet.h>
#include <sstream>

#define _XOPEN_SOURCE 700
#define	PORT 8080
#define	CRLF "\r\n" //carriage return and line feed
#define	PATH_MIME "/nfs/homes/niboukha/Desktop/webserv/extraFiles/mimetype.txt"
#define	BUF 1024
typedef std::map<std::string, std::vector<std::string> > mapStrVect;//stringToVectMap
typedef std::map<std::string, std::vector<std::string> >::value_type MapValueType;

enum	Stage
{
	REQLINE,
	REQHEADER,
	REQBODY,
	RESHEADER,
	RESBODY,
	RESEND
};

// #include "../includes/Request.hpp"
// #include "../includes/Response.hpp"
// #include "Client.hpp"
// #include "../includes/Get.hpp"
// #include "../includes/Post.hpp"
// #include "../includes/Delete.hpp"

#include "Exceptions.hpp"
#include "../includes/StringOperations.hpp"
#include "../includes/Utils.hpp"

#endif
