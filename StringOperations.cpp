/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringOperations.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:39:21 by shicham           #+#    #+#             */
/*   Updated: 2024/02/08 11:19:05 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

StringOperations::StringOperations()
{
}

StringOperations::~StringOperations()
{
}

bool    StringOperations::containsNonAlpha(std::string &str)
{
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;
}

std::string StringOperations::trim(const std::string& str, const std::string REMOVE)
{
    size_t first = str.find_first_not_of(REMOVE);
    size_t last = str.find_last_not_of(REMOVE);
    if (first == last)
        return ("");
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> StringOperations::parseString(std::string line)
{
    std::vector<std::string>    vectStrings;
    size_t                      start, end;
    std::string                 result;
    
    line = trim(line);
    start = end = 0;
    while ((start = line.find_first_not_of(" \t", end) != std::string::npos))
    {
        end = line.find_first_of(" \t", start);
        result = line.substr(start, end - start);
        vectStrings.push_back(result);
    }
    return vectStrings;
}