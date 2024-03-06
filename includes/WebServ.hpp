/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:53:55 by shicham           #+#    #+#             */
/*   Updated: 2024/03/05 22:02:36 by shicham          ###   ########.fr       */
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
#include <netdb.h>//mockil f trim 
#define _XOPEN_SOURCE 700
#define	PORT 8080
#define	CRLF "\r\n" //carriage return and line feed
#define	PATH_MIME "/nfs/homes/shicham/Desktop/web/extraFiles/mimetype.txt"
#define	BUF 1024

#define	ERROR_201 "/nfs/homes/shicham/Desktop/web/ErrorPages/201.html"
#define	ERROR_204 "/nfs/homes/shicham/Desktop/web/ErrorPages/204.html"
#define	ERROR_301 "/nfs/homes/shicham/Desktop/web/ErrorPages/301.html"
#define	ERROR_401 "/nfs/homes/shicham/Desktop/web/ErrorPages/401.html"
#define	ERROR_403 "/nfs/homes/shicham/Desktop/web/ErrorPages/403.html"
#define	ERROR_404 "/nfs/homes/shicham/Desktop/web/ErrorPages/404.html"
#define	ERROR_409 "/nfs/homes/shicham/Desktop/web/ErrorPages/409.html"
#define	ERROR_413 "/nfs/homes/shicham/Desktop/web/ErrorPages/413.html"
#define	ERROR_500 "/nfs/homes/shicham/Desktop/web/ErrorPages/500.html"
#define	ERROR_501 "/nfs/homes/shicham/Desktop/web/ErrorPages/501.html"
#define	ERROR_400 "/nfs/homes/shicham/Desktop/web/ErrorPages/400.html"
#define	ERROR_414 "/nfs/homes/shicham/Desktop/web/ErrorPages/414.html"
#define	ERROR_405 "/nfs/homes/shicham/Desktop/web/ErrorPages/405.html"

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
