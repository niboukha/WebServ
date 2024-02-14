/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:37:55 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/14 12:27:47 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "WebServ.hpp"
#include "Server.hpp"

class Request
{
    private :
        
        mapStrVect serverMethod;
        mapStrVect locationMethod;
        mapStrVect headersMethod;
        
        std::map<std::string, std::string>	errorPages; //exmpl : <404, path>
        
        std::string method;
        std::string pathHeader;
        std::string protocolVersion;


    public :

        Request ();
        ~Request ();

		Stage	parseRequest(std::string buffer);
        
        const mapStrVect    getLocationMethod() const;
        const std::string   getPathHeader() const;
        const std::string   getMethod() const;
        const std::string   getProtocolVersion() const;
		
		const std::map<std::string, std::string>&	getErrorPage() const;
        
};

#endif
