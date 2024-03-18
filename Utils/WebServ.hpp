/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ../Utils/WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:53:55 by shicham           #+#    #+#             */
/*   Updated: 2024/03/01 20:44:32 by shicham          ###   ########.fr       */
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
#include <csignal>
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
#include <errno.h>
#include <sys/select.h>
#include <list>
#include <netdb.h>
#include <sys/wait.h>
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define _XOPEN_SOURCE 700

#define	CRLF	  "\r\n" //carriage return and line feed

#define	PATH_CGI  "./upload/"
#define	PATH_MIME "./ErrorPages/mimetype.txt"

#define	ERROR_201 "./ErrorPages/201.html"
#define	ERROR_204 "./ErrorPages/204.html"
#define	ERROR_301 "./ErrorPages/301.html"
#define	ERROR_401 "./ErrorPages/401.html"
#define	ERROR_403 "./ErrorPages/403.html"
#define	ERROR_404 "./ErrorPages/404.html"
#define	ERROR_409 "./ErrorPages/409.html"
#define	ERROR_413 "./ErrorPages/413.html"
#define	ERROR_500 "./ErrorPages/500.html"
#define	ERROR_501 "./ErrorPages/501.html"
#define	ERROR_400 "./ErrorPages/400.html"
#define	ERROR_414 "./ErrorPages/414.html"
#define	ERROR_405 "./ErrorPages/405.html"
#define	ERROR_408 "./ErrorPages/408.html"
#define	ERROR_505 "./ErrorPages/505.html"
#define	ERROR_411 "./ErrorPages/411.html"
#define	ERROR_504 "./ErrorPages/504.html"
#define	ERROR_502 "./ErrorPages/502.html"

typedef std::map<std::string, std::vector<std::string> > mapStrVect;
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

enum	CgiStage
{
	INITCGI,
	WAITCGI,
	EXECUTECGI,
	ERRORCGI
};

#include "Exceptions.hpp"
#include "StringOperations.hpp"
#include "Utils.hpp"

#endif
