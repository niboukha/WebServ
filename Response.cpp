/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 09:39:23 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/10 11:33:22 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request &request) : req(request)
{
    
}

Response::~Response()
{
    
}

const Request	Response::getRequest() const
{
    return (req);
}

void   Response::fillResponse()
{
    if (req.getMethod() == "Get")
    {
        get = new Get(*this);
    }    
    // else if (req.getMethod() == "Post")
    //     post = new Post(*this);
    // else if (req.getMethod() == "Delete")
    //     delt = new Delete(*this);

    
}

