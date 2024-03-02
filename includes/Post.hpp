/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 11:11:11 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/27 15:56:04 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
#define POST_HPP

#include "WebServ.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Response.hpp"
class Response;

class	Post
{
	private :

		Response	&res;

		long long	size;
		bool		isMoved;
		int			sizeofRead;
		bool		enter;
		long long	uploadSize;
		long long	saveOffset;

	public :

		Post( Response &response );
		~Post( );

		std::string				responsHeader(Stage &stage);
		std::string				responsBody();

		void					requestedStatus(Stage &stage);



		void					chunkedTransfer(std::string body, Stage &stage);
		void					nonChunkedTransfer(Stage &stage);
		void					unsupportedUpload( );

		void					cgiPassCheck();
		void					directoryInRequest(std::string &path, std::ifstream	&file);
		std::string				conctRootUpload( std::string s );
		std::string				getExtensionFile();
		long long				maxBodySize();
		bool					isUploadPass( Stage &stage );

		const std::streampos	getSizeofRead() const;
};


#endif
