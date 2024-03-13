/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/13 11:34:03 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
#define GET_HPP

#include "../Utils/WebServ.hpp"
#include "../Utils/Utils.hpp"
#include "../Cgi/Cgi.hpp"

class Cgi;

class	Get
{
	private :
	
		Response					&response;
		Cgi 						cgi;
		
		std::vector<std::string>	vDir;
		std::string					fillAutoIndexFile;
		std::streampos				sizeofRead;
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

		void					statusOfFile( Stage& stage, CgiStage &cgiStage );
		void					responsHeader(std::string	&headerRes, Stage& stage, CgiStage &cgiStage);
		void					responsBody(std::string &bodyRes);

		const std::streampos&	getSizeofRead() const;
		
		void					directoryInRequest(std::string &path, std::ifstream &file, CgiStage &cgiStage);
		void					readListOfCurDirectory( CgiStage &cgiStage );
		void					stringOfDyrectories(std::vector<std::string> &vdir);		
		void					pathPermission( CgiStage &cgiStage);
		bool					cgiPassCheckment( CgiStage &cgiStage );
		void					parseFileCgi(std::string	&headerRes);


};

#endif
