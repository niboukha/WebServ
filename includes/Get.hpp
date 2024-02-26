/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/24 21:37:03 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
#define GET_HPP

#include "WebServ.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Response.hpp"

class Response;

class	Get
{
	private :
		Response					&response;
		std::string					fillAutoIndexFile;
		std::vector<std::string>	vDir;
		std::string					locationRes;
		
		int							fd;
		int							dirflag;
		int							sizeofRead;
		
	public :

		Get(Response &res);
		~Get();

		void		statusOfFile();

		std::string	directoryInRequest(std::string &file);

		std::string	readListOfCurDirectory();
		std::string	stringOfDyrectories(std::vector<std::string> &vdir);

		std::string	responsHeader();
		std::string	responsBody();

		const int&	getSizeofRead() const;
	
		class	DirectoryFailed : public std::exception
		{
			public :
				const char	*what() const throw();
		};

};

#endif
