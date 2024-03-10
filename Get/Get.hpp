/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 09:38:21 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/10 15:01:34 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
#define GET_HPP

#include "../Utils/WebServ.hpp"
#include "../Utils/Utils.hpp"
// #include "../Response/Response.hpp"
#include "../Cgi/Cgi.hpp"

// class Response;
class Cgi;

class	Get
{
	private :
	
		Response					&response;
		Cgi 						cgi;

		std::vector<std::string>	vDir;
		
		std::string					fillAutoIndexFile;
		std::string					locationRes;
		std::streampos				sizeofRead;
		bool						isMoved;
		std::ifstream				in;
		std::string					directories;
		CgiStage					cgiStage;
		
		class	DirectoryFailed : public std::exception
		{
			public :
				const char	*what() const throw();
		};
		
	public :

		Get(Response &res);
		~Get();

		void					statusOfFile( Stage& stage );
		void					responsHeader(std::string	&headerRes, Stage& stage);
		void					responsBody(std::string &bodyRes);

		const std::streampos&	getSizeofRead() const;
		
		void					directoryInRequest(std::string &path, std::ifstream &file);
		void					readListOfCurDirectory();
		void					stringOfDyrectories(std::vector<std::string> &vdir);		
		void					pathPermission();
		bool					cgiPassCheckment();
		void					parseFileCgi(std::string	&headerRes);


};

#endif
