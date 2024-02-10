/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/10 20:27:44 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
#define GET_HPP

#include "WebServ.hpp"

class Response;

class	Get
{
	private :
		Response					&response;
		std::string					statusCodeMsg;
		std::string					path;
		std::vector<std::string>	vDir;
		std::string					fillAutoIndexFile;

	public :
		
		Get(Response &res);
		~Get();
		
		void		openFile();
		
		std::string	getContentType();
		std::string	concatenatePath();
		std::string	directoryInRequest(std::string &file);
		std::string	concatenateIndexDirectory(std::string &file);
		
		void	readListOfCurDirectory();
		void	stringOfDyrectories(std::vector<std::string> &vdir);

		int		isDir(const char* file); // you should read about it

		char*	responseFill();

		class	DirectoryFailed : public std::exception
		{
			public :
				const char	*what() const throw();
		};

};


#endif
