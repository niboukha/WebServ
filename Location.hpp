/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 10:30:53 by shicham           #+#    #+#             */
/*   Updated: 2024/02/08 11:25:44 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "WebServ.hpp"

class Location
{
    private:
        mapStrVect locationData;
    public:
        Location();
        ~Location();
        void    setLocationData(mapStrVect& locData);
       const mapStrVect& getLocationData() const;
};


#endif