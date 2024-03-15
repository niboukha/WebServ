/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shicham <shicham@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:57:58 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/13 22:53:39 by shicham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Response/Response.hpp"

Cgi::Cgi( Response &res ) :	response			( res  ),
							env					( NULL ),
							uploadSize 			(  0   ),
							contentLengthLong	(  0   ),
							inter				( NULL ),
							pid					(  -1  ),
							inputFile			( NULL ),
							outputFile			( NULL )
{

}

Cgi::~Cgi( )
{
	std::map<std::string, std::string> :: iterator	it;
	int 											i;
	
	it = requestEnv.begin();
	i  = 0;
	for(; it != requestEnv.end(); it++, i++)
		delete [] env[i];
	delete [] env;
	if (pid != -1)
		kill(pid, SIGTERM);
}

int&	Cgi::getPid()
{
	return (pid);
}

void	Cgi::linkReqEnv()
{
	const std::map<std::string, std::string>	&header = response.getRequest().getHeaders();

	if (header.find("content-type")   != header.end())
		requestEnv["CONTENT_TYPE"]     = header.find("content-type")->second;
	if (header.find("content-length") != header.end())
		requestEnv["CONTENT_LENGTH"]   = header.find("content-length")->second;
	if (header.find("cookie")         != header.end())
		requestEnv["HTTP_COOKIE"]      = header.find("cookie")->second;
	if (header.find("user-agent")     != header.end())
		requestEnv["HTTP_USER_AGENT"]  = header.find("user-agent")->second;
	if (!response.getRequest().getMethod().empty())
		requestEnv["REQUEST_METHOD"]   = response.getRequest().getMethod();
	if (!response.getRequest().getQueryParameters().empty())
		requestEnv["QUERY_STRING"]     = response.getRequest().getQueryParameters();
	
	requestEnv["PATH_INFO"]            = response.getPath(); //diriha
	requestEnv["SCRIPT_FILENAME"]      = response.getPath(); //o diriha
	requestEnv["SCRIPT_NAME"]          = response.getPath(); //diriha
	requestEnv["SERVER_PROTOCOL"]      = "HTTP/1.1";
	requestEnv["GATEWAY_INTERFACE"]    = "CGI/1.1";
	
	requestEnv["REDIRECT_STATUS"]	   = "200";
	requestEnv["REMOTE_ADDR"]		   = "10.14.9.1"; //for now
	requestEnv["SERVER_PORT"]          = "8080"; //for now

}

void	Cgi::fillEnvirement()
{
	std::map<std::string, std::string> :: iterator	it;
	std::string										keyval;
	int												i;
	
	linkReqEnv();

	env = new char*[requestEnv.size() + 1];
	i   = 0;
	it  = requestEnv.begin();
	for(; it != requestEnv.end(); it++, i++)
	{
		keyval = it->first + "=" + it->second;
		env[i] = new char[keyval.length() + 1];
		for (size_t j = 0; j < keyval.length(); j++)
			env[i][j] = keyval[j];
		env[i][keyval.length()] = '\0';
	}
	env[i] = NULL;
}

long long	Cgi::maxBodySize( )
{
	const std::map<std::string, std::string>	&ser = response.getRequest().getServer();
	std::stringstream							s;
	long long									n;

	s << ser.find("client_max_body_size")->second;
	s >> n;
	return ( n );
}

void	Cgi::cgiBinary( )
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	if(!loc.find("cgi_pass")->second.front().empty())
	{
		if ( loc.find("cgi_pass")->second.front() == ".py" )
		{
			cgiBin = "/usr/bin/python3";
			inter  = (char*)"python3";
		}
		else if (loc.find("cgi_pass")->second.front() == ".php")
		{
			cgiBin = "/usr/bin/php-cgi";
			inter  = (char*)"php-cgi";
		}
	}
}

void 	Cgi::uploadBody(Stage &stage, std::string &reqBuff, CgiStage &cgiStage)
{
	const std::map<std::string, std::string>	&head = response.getRequest().getHeaders();

	if (!inputFile)
	{
		pathInput = pathInput + PATH_CGI + "input" + Utils::generateRundFile();
		inputFile = fopen(pathInput.c_str(), "wr");
		contentLengthLong = Utils::stringToLong(head.find("content-length")->second);
		if (maxBodySize() < contentLengthLong)
		{
			cgiStage = ERRORCGI;
			stage    = RESHEADER;
			response.throwNewPath("413 Request Entity Too Large", "413");
		}
	}
	if (static_cast<long long>(uploadSize + reqBuff.size()) > contentLengthLong)
	{
		fwrite ( std::string (reqBuff, 0, contentLengthLong - uploadSize).c_str(),
					sizeof(char), contentLengthLong - uploadSize, inputFile);
		stage = RESHEADER;
		fclose(inputFile);
		reqBuff.clear();
		return ;
	}
	fwrite ( reqBuff.c_str(), sizeof(char), reqBuff.size(), inputFile);
	uploadSize += reqBuff.size();
	if (uploadSize == contentLengthLong)
	{
		stage = RESHEADER;
		fclose(inputFile);
	}
	reqBuff.clear();
}

// void	Cgi::getStatusCgi()
// {
// 	std::ifstream	outfile(pathOutput);
// 	std::string		s;
	
// 	while (std::getline(outfile, s))
// 	{
		
// 	}
// }

void	Cgi::waitCgi(Stage &stage, int &pid, CgiStage &cgiStage)
{
	int			status;
	int			exitStatus;
	std::string statusCode;
	
	cgiStage = WAITCGI;
	if (waitpid(pid, &status, 0) != 0 && WIFEXITED(status))
	{
		exitStatus = WEXITSTATUS(status);
		if (exitStatus != 0)
		{
			cgiStage = ERRORCGI;
			stage    = RESHEADER;
			response.throwNewPath("500 Internal Server Error", "500");
		}
		cgiStage = EXECUTECGI;
		stage    = RESHEADER;
		// getStatusCgi();
		statusCode = "200 ok";
		response.setStatusCodeMsg(statusCode);
		throw (pathOutput);
	}
}

void	Cgi::executeCgi( std::string &reqBuff, Stage &stage, CgiStage &cgiStage )
{
	fillEnvirement ( );
	if (stage < RESHEADER)
		uploadBody(stage, reqBuff, cgiStage);
	cgiBinary( );

	if (!outputFile)
	{
		pathOutput = pathOutput + PATH_CGI + "output" + Utils::generateRundFile();
		outputFile = fopen(pathOutput.c_str(), "w");
	}
	char *arr[] = {inter, (char *)response.getPath().c_str(), NULL};
	pid = fork();
	if (pid == 0)
	{
		inputFile = freopen(pathInput.c_str(), "r" , stdin);
		outputFile = freopen(pathOutput.c_str(), "w", stdout);
		execve(cgiBin.c_str(), arr, env);
	}
	// if (inputFile != NULL)
	// 	fclose(inputFile);
	// fclose(outputFile);
	waitCgi(stage, pid, cgiStage);
}
