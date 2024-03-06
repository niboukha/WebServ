/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 17:59:44 by shicham           #+#    #+#             */
/*   Updated: 2024/03/05 05:28:32 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

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

class UnknownServerDirective : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("config file: unknown server  directive");
        }   
};

class InvalidDirectiveArgument : public std::exception
{
    public:
        const char* what() const throw()
        {
            return ("config file: invalid directive argument");
        }   
};

class DirectiveAlreadyExist : public std::exception
{
    public:
        const char* what() const throw()
        {
            return ("config file: directive already exist");
        }  
};

class UnknownLocationDirective : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("config file: unknown location  directive");
        }   
};

class InvalidNumberOfArguments : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("config file: invalid number of arguments in directive");
        }   
};

class MissingServerDirectives : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("config file: some necessary directives missing in server block");
        }   
};

class MissingLocationDirectives : public std::exception
{
     public:
        const char* what() const throw()
        {
            return ("config file: some necessary directives missing in location block");
        }   
};

class InvalidLocationDirective : public std::exception
{
    public:
        const char* what() const throw()
        {
            return ("config file: invalid location directive");
        }   
};

#endif
