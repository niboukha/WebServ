/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:54:43 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/26 07:18:56 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "WebServ.hpp"

namespace Utils
{
	int		isDir( const char* file );
	int		isFile( const char* file );
	void	trimString( std::string &s );

	std::vector<std::string>	moreThanKey( std::string s );
	std::string					generateRundFile();
	bool						isFdOpen(int fd);
	int							getLength(std::string s);
	long long					stringToInt(std::string s);

};

#endif
