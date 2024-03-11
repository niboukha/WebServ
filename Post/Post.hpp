/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 11:11:11 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/10 21:18:28 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
#define POST_HPP

#include "../Utils/WebServ.hpp"
#include "../Utils/Utils.hpp"
#include "../Response/Response.hpp"

class Cgi;

class	Post
{
	private :

		Response		&res;
		Cgi 			cgi;

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

		void					responsHeader(Stage &stage, std::string &reqBuff, std::string &headerRes, CgiStage &cgiStage);
		void					responsBody(std::string &bodyRes);

		void					requestedStatus(Stage &stage, std::string &reqBuff, CgiStage &cgiStage);

		void					chunkedTransfer(std::string &reqBuff, Stage &stage);
		void					nonChunkedTransfer(Stage &stage, std::string &reqBuff);
		void					unsupportedUpload( std::string &reqBuff, Stage &stage, CgiStage &cgiStage );

		void					cgiPassCheckment( std::string &reqBuff, Stage &stage, CgiStage &cgiStage );
		void					directoryInRequest(std::string &reqBuff, std::string &path, std::ifstream	&file, Stage &stage, CgiStage &cgiStage);
		std::string				conctRootUpload( std::string s );
		std::string				getExtensionFile( CgiStage &cgiStage );
		long long				maxBodySize();
		bool					isUploadPass( Stage &stage );

		const std::streampos	getSizeofRead() const;
};


#endif
