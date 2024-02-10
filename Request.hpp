/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:37:55 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/10 09:41:52 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "WebServ.hpp"
#include "Server.hpp"
#include "Response.hpp"

class Response;

class Request
{
    private :
        
        mapStrVect serverMethod;
        mapStrVect locationMethod;
        mapStrVect headersMethod;
        
        std::string method;
        std::string pathHeader;
        
        Response	*response;
    
        
    public :

        Request ();
        ~Request ();

		void	parseRequest(char *req);

        void    setLocationMethod(mapStrVect loc);
        void    setPathHeader(std::string pHeader);
        void    setMethod(std::string& m);
        
        const mapStrVect    getLocationMethod() const;
        const std::string   getPathHeader() const;
        const std::string   getMethod() const;
        const std::string   getProtocolVersion();
        
};

#endif
