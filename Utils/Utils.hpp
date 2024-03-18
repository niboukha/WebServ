/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:54:43 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/17 02:53:48 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "../Utils/WebServ.hpp"

namespace Utils
{
	std::vector<std::string>	moreThanKey( std::string s );

	int							isDir	  ( const char* file );
	int							isFile	  ( const char* file );

	void						trimString( std::string &s );
	void						toLower	  (std::string &s);

	std::string					generateRundFile();
	std::string					longToString    (long long len);
	std::string					getcwd_string   ();

	long long					getLength   (std::string s);
	long long					stringToLong(std::string s);
};

#endif
