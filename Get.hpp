/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 16:17:46 by niboukha         ###   ########.fr       */
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
		int							fd;

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

		class	DirectoryFailed : public std::exception
		{
			public :
				const char	*what() const throw();
		};

};


#endif
