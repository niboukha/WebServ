/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:38:41 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/10 15:04:57 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "../Utils/WebServ.hpp"
#include "../Utils/Utils.hpp"
// #include "../Post/Post.hpp"
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

		
	public:

		Cgi (Response &res);
		~Cgi();

		void		executeCgi(std::string &reqBuff, Stage &stage, CgiStage &cgiStage);
		void		uploadBody(Stage &stage, std::string &reqBuff);
		void		linkReqEnv( );
		void		fillEnvirement( );
		long long	maxBodySize( );
		void		cgiBinary(Stage &stage);
		void		readaOutfile();
		void		waitCgi(Stage &stage, int &pid, CgiStage &cgiStage);

		int&		getPid();
		void		setPid(const int p);

};

#endif
