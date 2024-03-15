/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:38:41 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/14 12:14:14 by niboukha         ###   ########.fr       */
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
		std::string							pathInput;
		std::string							pathOutput;
		std::string							cgiBin;
		char								*inter;
		pid_t								pid;
		FILE* 								inputFile;
		FILE* 								outputFile;
		bool								hasNewLine;

		
	public:

		Cgi (Response &res);
		~Cgi();

		void	setHasNewLine(const bool& newline);

		int&		getPid();
		const bool&	getHasNewLine() const;
	
		void		executeCgi(std::string &reqBuff, Stage &stage, CgiStage &cgiStage);
		void		uploadBody(Stage &stage, std::string &reqBuff, CgiStage &cgiStage);
		void		linkReqEnv( );
		void		fillEnvirement( );
		void		cgiBinary( );
		void		waitCgi(Stage &stage, int &pid, CgiStage &cgiStage);
		void		getStatusCgi();
		void		checkValidStatus(std::string &status);

		long long	maxBodySize( );

};

#endif
