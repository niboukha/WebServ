/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:38:41 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/17 01:59:31 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "../Utils/WebServ.hpp"
#include "../Utils/Utils.hpp"
#include "../Response/Response.hpp"

class	Response;

class	Cgi
{
	private:
		Response							&response;

		std::map<std::string, std::string>	requestEnv;
		char								**env;
		long long							uploadSize;
		long long							contentLengthLong;
		std::string							cgiBin;
		char								*inter;
		pid_t								pid;
		FILE* 								inputFile;
		FILE* 								outputFile;
		bool								hasNewLine;
		std::string							currDir;
		long long							size;			
		long long							maxBody;
		
	public:

		Cgi (Response &res);
		~Cgi( );

		void		executeCgi( std::string &reqBuff, Stage &stage, CgiStage &cgiStage );
		void		uploadBody( Stage &stage, std::string &reqBuff, CgiStage &cgiStage );
		void 		uploadByChunks( Stage &stage, std::string &reqBuff, CgiStage &cgiStage );
		void 		uploadByContentLength( Stage &stage, std::string &reqBuff, CgiStage &cgiStage );

		void		setHasNewLine( const bool& newline );

		int&		getPid( );
		const bool&	getHasNewLine() const;

		void		linkReqEnv( );
		void		fillEnvirement( );
		void		cgiBinary( );
		void		waitCgi(Stage &stage, int &pid, CgiStage &cgiStage );
		void		getStatusCgi(Stage &stage, CgiStage &cgiStage );

		long long	maxBodySize( );
};

#endif
