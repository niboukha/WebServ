/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringOperations.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:39:21 by shicham           #+#    #+#             */
/*   Updated: 2024/02/23 11:58:46 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/StringOperations.hpp"

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

std::vector<std::string>    StringOperations::split(std::string s, std::string delim)
{
    size_t  posStart = 0, posEnd = 0;
    std::vector<std::string>    vectRes;
    std::string                 token;

    while (((posStart = s.find_first_not_of(delim, posEnd)) != std::string::npos))
    {
        posEnd = s.find_first_of(delim, posStart);
        token = s.substr(posStart, posEnd - posStart);
        vectRes.push_back(token);
    }
    return vectRes;
}