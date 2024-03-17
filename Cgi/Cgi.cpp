/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niboukha <niboukha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:57:58 by niboukha          #+#    #+#             */
/*   Updated: 2024/03/17 04:01:10 by niboukha         ###   ########.fr       */
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
							outputFile			( NULL ),
							hasNewLine			( false)
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
	{
		kill(pid, SIGTERM);
		waitpid(pid, 0, 0);
	}
}

void	Cgi::setHasNewLine(const bool& newline)
{
	hasNewLine = newline;
}


int&	Cgi::getPid()
{
	return (pid);
}

const bool&	Cgi::getHasNewLine() const
{
	return (hasNewLine);
}

void	Cgi::linkReqEnv()
{
	const std::map<std::string, std::string>	&header = response.getRequest().getHeaders();
	const std::map<std::string, std::string>	&server = response.getRequest().getServer();
	size_t										found;
	std::string									path;

	path = response.getPath();
	found = path.find_last_of("/");
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
	requestEnv["SERVER_PORT"]          = server.find("port")->second;
	requestEnv["PATH_INFO"]            = response.getPath();
	requestEnv["SCRIPT_FILENAME"]      = response.getPath();
	requestEnv["SCRIPT_NAME"]          = path.substr(found + 1);
	requestEnv["SERVER_PROTOCOL"]      = "HTTP/1.1";
	requestEnv["GATEWAY_INTERFACE"]    = "CGI/1.1";
	requestEnv["REDIRECT_STATUS"]	   = "200";

	requestEnv["REMOTE_ADDR"]		   = "10.14.9.1"; //for now
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
	std::string									s;

	if (!inputFile)
	{
		response.setPathInput(s + PATH_CGI + "input" + Utils::generateRundFile());
		inputFile = fopen(response.getPathInput().c_str(), "wrb");
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

void	Cgi::getStatusCgi(Stage &stage, CgiStage &cgiStage)
{
	std::ifstream	outfile(response.getPathOutput().c_str());
	std::string		s;
	std::string		status;
	size_t			found;
	size_t			foundContentType;
	std::string 	statusCode;
	
	foundContentType = std::string::npos;
	while (std::getline(outfile, s))
	{
		Utils::toLower(s);
		if (s.empty() || s == "\r")
		{
			hasNewLine = true;
			break;
		}
		if (s[s.length() - 1] != '\r')
			break;
		if (foundContentType == std::string::npos)
			foundContentType = s.find("content-type:");
		found = s.find("status:");
		if (found != std::string::npos and found == 0)
		{
			status = std::string (s, s.find_first_of(":") + 1);
			Utils::trimString( status );
			if (status.empty())
			{
				statusCode = "200 ok";
				response.setStatusCodeMsg(statusCode);
			}
			response.setStatusCodeMsg(status);
		}
		s.clear();
	}
	if (hasNewLine == false or foundContentType == std::string::npos)
	{
		cgiStage = ERRORCGI;
		stage    = RESHEADER;
		response.throwNewPath("502 Bad Gateway", "502");
	}
}

void	Cgi::waitCgi(Stage &stage, int &pid, CgiStage &cgiStage)
{
	int			status;
	int			exitStatus;
	std::string statusCode;
	
	cgiStage = WAITCGI;
	if (waitpid(pid, &status, WNOHANG) != 0 && WIFEXITED(status))
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
		statusCode = "200 ok";
		response.setStatusCodeMsg(statusCode);
		getStatusCgi(stage, cgiStage);
		throw (response.getPathOutput());
	}
}

void	Cgi::executeCgi( std::string &reqBuff, Stage &stage, CgiStage &cgiStage )
{
	const mapStrVect	&loc = response.getRequest().getLocation();
	std::string			path;
	std::string			pathExt;
	size_t				found;
	size_t				foundPoint;

	fillEnvirement ( );
	if (stage < RESHEADER)
		uploadBody(stage, reqBuff, cgiStage);
	else
	{
		pathExt    = response.getPath();
		foundPoint = pathExt.find_last_of(".");
		
		if ((foundPoint != std::string::npos 
			and std::string (pathExt, foundPoint) != loc.find("cgi_pass")->second.front()) 
			or foundPoint == std::string::npos)
			{
				std::cout << "hna\n";
				cgiStage = ERRORCGI;
				stage    = RESHEADER;
				response.throwNewPath("403 forbidden", "403");
			}
			
		cgiBinary( );

		response.setPathOutput(path + PATH_CGI + "output" + Utils::generateRundFile());
		char *arr[] = {inter, (char *)response.getPath().c_str(), NULL};
		pid = fork();
		if (pid == 0)
		{
			inputFile  = freopen(response.getPathInput().c_str(), "rb" , stdin);
			outputFile = freopen(response.getPathOutput().c_str(), "wb", stdout);
			path = response.getPath();
			found = path.find_last_of("/");
			if (chdir(path.substr(0, found).c_str()) == -1)
			{
				cgiStage = ERRORCGI;
				stage    = RESHEADER;
				response.throwNewPath("500 Internal Server Error", "500");
			}
			
			execve(cgiBin.c_str(), arr, env);
			exit(EXIT_FAILURE);
		}
		waitCgi(stage, pid, cgiStage);
	}
}
