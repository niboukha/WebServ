/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/23 08:47:33 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
#define GET_HPP

#include "WebServ.hpp"
#include "Utils.hpp"
#include "Response.hpp"

class Response;

class	Get
{
	private :
		Response					&response;
		std::string					fillAutoIndexFile;
		std::vector<std::string>	vDir;
		// int							fd;
		int							dirflag;
		std::string					locationRes;
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

		void		UpdateStatusCode(std::string &s);

		const int&		getSizeofRead() const;
	
		class	DirectoryFailed : public std::exception
		{
			public :
				const char	*what() const throw();
		};

};


#endif
