/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/14 14:13:12 by niboukha         ###   ########.fr       */
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
		
		// std::string					statusCodeMsg;
		// std::string					path;
		std::string					fillAutoIndexFile;
		std::vector<std::string>	vDir;

	public :
		
		Get(Response &res);
		~Get();
		
		void		statusOfFile();
		
		std::string	directoryInRequest(std::string &file);
		
		void	readListOfCurDirectory();
		void	stringOfDyrectories(std::vector<std::string> &vdir);

		std::string	responsHeader();
		std::string	responsBody();

		class	DirectoryFailed : public std::exception
		{
			public :
				const char	*what() const throw();
		};

};


#endif
