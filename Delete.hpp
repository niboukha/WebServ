/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 11:11:44 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 13:03:35 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
#define DELETE_HPP

#include "WebServ.hpp"
#include "Response.hpp"

class Response;

class	Delete
{
	private :

		Response	&res;

	public :

		Delete(Response &response);
		~Delete();

		void	WriteAccess();
		void	directoryPath(struct stat st, std::string &pt);
		void	filePath(std::string &s);
		void	statusOfRequested();

		std::string	nestedDirectories(std::string s);


};


#endif
