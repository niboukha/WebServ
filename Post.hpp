/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 11:11:11 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/18 18:35:15 by niboukha         ###   ########.fr       */
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

	public :

		Post( Response &response );
		~Post( );

		std::string	responsHeader();
		std::string	responsBody();
		
		void	requestedStatus();

		void	uploadedFile();

		void	chunkedTransfer();
		void	nonChunkedTransfer();

		void	isUploadPass();
		
		std::string	conctRootUpload( std::string s );
};


#endif
