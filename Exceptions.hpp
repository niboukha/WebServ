/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:59:44 by shicham           #+#    #+#             */
/*   Updated: 2024/02/08 13:20:21 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "WebServ.hpp"

class InvalidArguments : public std::exception
{

    public:
        const char* what() const throw()
        {
            return ("invalid number of arguments !!");
        }    
};

class FailedToOpenFile : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("couldn't open file");
        }    
};

class SyntaxError : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("config file: syntax error");
        }    
};

class UnknownDirective : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("config file: unknown directive");
        }   
};

class InvalidDirectiveParameter : public std::exception
{
    public:
        const char* what() const throw()
        {
            return ("config file: invalid directive parameter");
        }   
};

#endif