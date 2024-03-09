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

#include "../Utils/WebServ.hpp"
#include "../Utils/Utils.hpp"
#include "../Response/Response.hpp"
class Response;

class	Post
{
	private :

		Response		&res;

		long long		size;
		bool			isMoved;
		int				sizeofRead;
		bool			enter;
		long long		uploadSize;
		long long		contentLengthLong;
		std::ofstream	UploadFile;
		std::ifstream	in;
		long long		maxBody;

	public :

		Post( Response &response );
		~Post( );

		void					responsHeader(Stage &stage, std::string &reqBuff, std::string &headerRes);
		void					responsBody(std::string &bodyRes);

		void					requestedStatus(Stage &stage, std::string &reqBuff);

		void					chunkedTransfer(std::string &reqBuff, Stage &stage);
		void					nonChunkedTransfer(Stage &stage, std::string &reqBuff);
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
