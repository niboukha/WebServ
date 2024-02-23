/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringOperations.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:39:23 by shicham           #+#    #+#             */
/*   Updated: 2024/02/22 07:05:52 by niboukha         ###   ########.fr       */
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
        static bool                     containsNonAlpha(std::string &str);
        static  std::vector<std::string>    split(std::string s, std::string delim);
};

#endif