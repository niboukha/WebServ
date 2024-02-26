/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 11:11:11 by niboukha          #+#    #+#             */
/*   Updated: 2024/02/26 18:09:47 by niboukha         ###   ########.fr       */
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
		int			fd;
		int			dirflag;
		int			sizeofRead;
		bool		enter;
		long long	uploadSize;

	public :

		Post( Response &response );
		~Post( );

		const int&	getSizeofRead() const;

		std::string	responsHeader(Stage &stage);
		std::string	responsBody();

		void	requestedStatus(Stage &stage);

		void	chunkedTransfer(std::string body, Stage &stage);
		void	nonChunkedTransfer(Stage &stage);
		void	unsupportedUpload( );

		bool	isUploadPass( Stage &stage );
	
		long long	maxBodySize();
		void		directoryInRequest(std::string &path, std::ifstream	&file);

		void		cgiPassCheck();
		std::string	conctRootUpload( std::string s );

		std::string	getExtensionFile();

};


#endif
