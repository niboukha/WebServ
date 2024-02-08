/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 10:30:57 by shicham           #+#    #+#             */
/*   Updated: 2024/02/07 11:16:27 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
}

Location::~Location()
{
}
void    Location::setLocationData(mapStrVect& locData)
{
    locationData = locData;
}

const mapStrVect& Location::getLocationData() const
{
    return locationData;
}