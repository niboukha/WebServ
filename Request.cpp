/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:42:49 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/09 08:32:26 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request ()
{
    
}

Request::~Request ()
{
    
}

void    Request::setMethod(std::string& m)
{
    method = m;
}

const std::string&  Request::getMethod() const
{
    return (method);
}

const mapStrVect    Request::getLocationPath() const
{
    
}


