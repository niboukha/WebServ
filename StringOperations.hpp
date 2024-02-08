/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringOperations.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:39:23 by shicham           #+#    #+#             */
/*   Updated: 2024/02/08 10:29:04 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGOPERATIONS_HPP
#define STRINGOPERATIONS_HPP


#include "WebServ.hpp"

class StringOperations
{
    public:
        StringOperations();
        ~StringOperations();
        static std::string trim(const std::string& str, const std::string REMOVE = "  \n\r\t");
        static std::vector<std::string> parseString(std::string line);
        static bool                     containsNonAlpha(std::string &str);
};

#endif