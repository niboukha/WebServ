/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   #include "../Delete/Delete.hpp"                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 11:11:44 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 16:13:09 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
#define DELETE_HPP

#include "../Utils/WebServ.hpp"
#include "../Response/Response.hpp"

class Response;

class	Delete
{
	private :

		Response		&res;
		
		std::streampos	sizeofRead;
		std::ifstream	in;
		bool			isReal;

	public :

		Delete(Response &response);
		~Delete();

		void	responsHeader(std::string &headerRes);
		void	responsBody(std::string &bodyRes);

		const std::streampos&	getSizeofRead() const;

		void		directoryPath(DIR *pDir, struct stat st, std::string &pt);
		void		filePath(std::string &s, struct stat st, DIR *pDir);
		void		statusOfRequested();
		void		pathOfSentPage(std::string s, std::string code);
		std::string	nestedDirectories(std::string s, struct stat statPath);


		void		deleteBasePath(std::string s, struct stat statPath);

};

#endif
