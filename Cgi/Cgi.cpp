/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:57:58 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/10 20:37:36 by niboukha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Response/Response.hpp"

Cgi::Cgi(Response &res) :	response(res),
							env(NULL),
							uploadSize ( 0 ),
							contentLengthLong (0),
							inter (NULL),
							pid (-1),
							inputFile (NULL),
							outputFile (NULL)
{

}

Cgi::~Cgi()
{
	delete [] env; //leaks
}

int&	Cgi::getPid()
{
	return (pid);
}

void		Cgi::setPid(const int p)
{
	pid = p;
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
	
	requestEnv["SERVER_PROTOCOL"]      = "HTTP/1.1";
	requestEnv["GATEWAY_INTERFACE"]    = "CGI/1.1";

	
	requestEnv["REDIRECT_STATUS"]	   = "200";//for now
	requestEnv["REMOTE_ADDR"]		   = "10.14.9.1"; //for now
	requestEnv["SERVER_PORT"]          = "8080"; //for now

	
	requestEnv["PATH_INFO"]            = response.getPath();
	requestEnv["SCRIPT_FILENAME"]      = response.getPath();
	requestEnv["SCRIPT_NAME"]          = response.getPath();

}

void	Cgi::fillEnvirement()
{
	std::map<std::string, std::string> :: iterator	it;
	std::string										keyval;
	int												i;
	
	linkReqEnv();
	env = new char*[requestEnv.size() + 1];

	i  = 0;
	it = requestEnv.begin();
	for(; it != requestEnv.end(); it++)
	{
		keyval = it->first + "=" + it->second;
		env[i] = new char[keyval.length() + 1];
		for (size_t j = 0; j < keyval.length(); j++)
			env[i][j] = keyval[j];
		env[i][keyval.length()] = '\0';
		i++;
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

void	Cgi::cgiBinary(Stage &stage, CgiStage &cgiStage)
{
	const std::string	&pt   = response.getPath();
	size_t				found = pt.find_last_of(".");

	std::cout << "pathhhh -> " << pt << "\n";
	if (found != std::string::npos)
	{
		if (pt.substr(found) == ".py")
		{
			cgiBin = "/usr/bin/python3";
			inter  = (char*)"python3";
		}
		else if (pt.substr(found) == ".php")
		{
			cgiBin = "/usr/bin/php-cgi";
			inter  = (char*)"php-cgi";
		}
	}
	if (cgiBin.empty())
	{
		cgiStage = ERRORCGI;
		stage    = RESHEADER;
		response.throwNewPath("403 forbiden", "403");//chechik
	}
}

void 	Cgi::uploadBody(Stage &stage, std::string &reqBuff, CgiStage &cgiStage)
{
	const std::map<std::string, std::string>	&head = response.getRequest().getHeaders();

	if (!inputFile)
	{
		pathInput = pathInput + PATH_CGI + Utils::generateRundFile();
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
		pathInput.clear();
		fclose(inputFile);
	}
	reqBuff.clear();
}

void	Cgi::waitCgi(Stage &stage, int &pid, CgiStage &cgiStage)
{
	int			status;
	int			exitStatus;
	std::string statusCode;
	
	cgiStage = WAITCGI;
	if (waitpid(pid, &status, 0) != 0 && WIFEXITED(status))
	{
		exitStatus = WEXITSTATUS(status);
		if (exitStatus == 500)
		{
			cgiStage = ERRORCGI;
			stage    = RESHEADER;
			response.throwNewPath("500 Internal Server Error", "500"); //checkkkkkkkk
		}
		cgiStage = EXECUTECGI;
		stage = RESHEADER;
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
	cgiBinary(stage, cgiStage);

	if (!outputFile)
	{
		pathOutput = pathOutput + PATH_CGI + Utils::generateRundFile();
		outputFile = fopen(pathOutput.c_str(), "wr");
		// fclose (outputFile);
	}
	char *arr[] = {inter, (char *)response.getPath().c_str(), NULL};
	pid = fork();
	if (pid == 0)
	{
		inputFile = freopen(pathInput.c_str(), "wr" , stdin);
		outputFile = freopen(pathOutput.c_str(), "wr", stdout);
		execve(cgiBin.c_str(), arr, env);
		std::cerr << "===> execve faild\n";
	}
	if (inputFile != NULL)
		fclose(inputFile);
	fclose(outputFile);
	waitCgi(stage, pid, cgiStage);
}
