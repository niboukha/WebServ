/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 11:11:11 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/21 16:06:27 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
#define POST_HPP

#include "WebServ.hpp"
#include "Utils.hpp"
#include "Response.hpp"

class Response;

class	Post
{
	private :

		Response	&res;
		size_t		size;
		int			fd;
		int			dirflag;
		
	public :

		Post( Response &response );
		~Post( );

		std::string	responsHeader(int stage);
		std::string	responsBody();
		
		void	requestedStatus(int stage);
		
		void	chunkedTransfer(std::string body);
		void	nonChunkedTransfer();
		void	unsupportedUpload();

		bool	isUploadPass();
		
		int		getLength(std::string s);
		size_t	maxBodySize();
		void	directoryInRequest(std::string &file);

		void	cgiPassCheck();
		std::string	conctRootUpload( std::string s );
};


#endif
