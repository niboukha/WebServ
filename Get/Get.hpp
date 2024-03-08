/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/27 11:36:50 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
#define GET_HPP

#include "../Utils/WebServ.hpp"
#include "../Utils/Utils.hpp"
#include "../Response/Response.hpp"

class Response;

class	Get
{
	private :
	
		Response					&response;
		std::vector<std::string>	vDir;
		
		std::string					fillAutoIndexFile;
		std::string					locationRes;
		std::streampos				sizeofRead;
		bool						isMoved;
		std::ifstream				in;
		std::string					directories;
		
		class	DirectoryFailed : public std::exception
		{
			public :
				const char	*what() const throw();
		};
		
	public :

		Get(Response &res);
		~Get();

		void					statusOfFile();
		void					responsHeader(std::string	&headerRes);
		void					responsBody(std::string &bodyRes);

		const std::streampos&	getSizeofRead() const;
		
		void					directoryInRequest(std::string &path, std::ifstream &file);
		void					readListOfCurDirectory();
		void					stringOfDyrectories(std::vector<std::string> &vdir);		
		void					pathPermission();
};

#endif
