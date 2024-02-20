#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <fstream>
#include <vector>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <sstream> // for string streams
#include <string>  // for string
// int main()
// {
// 	std::ofstream	file;

// 	file.open("file.html");
// 	char	cwd[PATH_MAX];
// 	DIR		*pDir;
// 	struct dirent *pDirent;

// 		if (!getcwd(cwd, sizeof(cwd)))
// 			std::cout << "fff\n";
//         std::cout << cwd << "\n";
// 		pDir = opendir(cwd);
// 		if (!pDir)
//            	std::cout << "rrr\n";
// 		file << "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>Index of/</h1>\n<hr />\n";
// 		while ((pDirent = readdir(pDir)))
// 			file <<  "<p><a href=\"" << pDirent->d_name << "\">" << pDirent->d_name << "</a></p>" "\n";
// 		file << "\n</body>\n</html>\n";
// 		closedir(pDir);
// }

// void	trimString( std::string &s )
// {
// 	size_t	foundFisrt;
// 	size_t	foundLast;

// 	foundFisrt = s.find_last_not_of( " \t" );
// 	foundLast  = s.find_first_not_of( " \t" );
// 	if ( foundFisrt != std::string::npos )
// 	{
// 		s = s.substr( 0, foundFisrt + 1 );
//     	s = s.substr( foundLast );
// 	}
// 	else
// 		s.clear();
// }

// std::vector<std::string>	moreThanKey(std::string s)
// {
// 	std::vector<std::string>	vect;
// 	std::string					str;

// 	trimString(s);
// 	for (size_t i = 0 ; i < s.length(); i++)
// 	{
// 		if (s[i] != ' ' and s[i] != ';')
// 			str += s[i];
// 		if ((s[i] == ' ' or s[i] == ';') and str.length() > 0)
// 		{
// 			vect.push_back(str);
// 			str.clear();
// 		}
// 	}
// 	return (vect);
// }

// int main()
// {
// 	std::string path = "/nfs/homes/niboukha/Desktop/webServ/mimetype.txt";
// 	std::ifstream	file("/nfs/homes/niboukha/Desktop/webServ/mimetype.txt");

// 	std::map<std::string, std::string>	type;
// 	std::vector<std::string>	vec;
// 	std::string	s;
// 	std::string	value;
// 	size_t		found;

// 	std::getline(file, s);

// 	while ( std::getline( file, s ) )
// 	{
// 		trimString( s );
// 		found = s.find_first_of( " \t" );
// 		value = s.substr(0, found + 1);
// 		vec = moreThanKey(s.substr(found + 1));
// 		for (size_t i = 0; i < vec.size(); i++)
// 			type[vec[i]] = value;
// 	}

// 	std::string	ret;
// 	std::string	r;

// 	found = path.find_last_of( "." );

// 	if (found != std::string::npos)
// 	{
// 		r = path.substr(found + 1);
// 		std::cout << r << "\n";
// 		ret = type[r];
// 	}

// 	long int	sz;

// 	FILE* fp = fopen(path.c_str(), "r");
//     fseek(fp, 0L, SEEK_END);
//     sz = ftell(fp);
//     fclose(fp);
// 	std::cout << sz << "\n";
// 	// std::map<std::string, std::string>::iterator	it = type.begin();
// 	// for (; it != type.end(); it++)
// 	// {
// 	// 	std::cout << it->first << " " << it->second << "\n";
// 	// }

// }

#include <cstdlib>
#include <iostream>
#include <time.h>

void WriteAccess()
{
	struct stat st;
	std::string s;

	if (st.st_mode & S_IWUSR)
	{
		s = "500 Internal Server Error";
		throw("500");
	}
	else
	{
		s = "403 Forbidden";
		throw("403");
	}
}

void directoryPath(struct stat st, std::string pt)
{
	std::string err;

	if (pt[pt.length() - 1] != '/')
	{
		err = "409 conflict";
		throw("409");
	}
	if (!(st.st_mode & S_IWUSR))
	{
		err = "403 Forbidden";
		throw("403");
	}
	if ((st.st_mode & S_IWUSR) && std::remove(pt.c_str()))
	{
		err = "500 Internal Server Error";
		throw("500");
	}
	else
	{
		err = "204 No Content";
		std::cout << err << "\n";
	}
}

void filePath(std::string s)
{
	std::string err;
	if (!std::remove(s.c_str()))
	{
		err = "204 No Content";
		std::cout << err << "<<\n";
	}
}

#include <sys/types.h>
#include <dirent.h>

std::string fun(std::string s, struct stat statPath)
{
	DIR *pDir;
	struct dirent *pDirent;
	std::string str;

	pDir = opendir(s.c_str());

	str = s;
	while ((pDirent = readdir(pDir)))
	{
		s = pDirent->d_name;
		if (s[0] == '.')
			continue;
		s = str + pDirent->d_name;
		std::cout << s << "\n";
		if (!stat(s.c_str(), &statPath))
		{
			if (S_ISDIR(statPath.st_mode))
			{
				s += "/";
				fun(s, statPath) = s;
				directoryPath(statPath, s);
			}
			else if (S_ISREG(statPath.st_mode))
				filePath(s);
		}
		else
		{
			s = "404 Not found";
			throw("404");
		}
	}
	return (s);
}

int	getLength(std::string s)
{
	std::stringstream	st;
	int					n;

	st << s;
	st >> std::hex >> n;
	return (n);
}

int main()
{
	std::ofstream	outfile;
	int				len;
	std::string		lrn;
	std::string		buffApp;
	size_t			foundlen;
	size_t			foundbuff;
	std::string		body = "4--wiki--5--pedia--E-- in--chunks.--5--nis----";
	int flag = 0;
	outfile.open("file.txt", std::ios_base::app);
	while (body.length())
	{
		if (flag == 0)
		{
			foundlen = body.find("--");
			if (foundlen != std::string::npos)
				lrn = body.substr(0, foundlen);
			len = getLength(lrn);
		}


		std::cout << "len     >>> " << len << " |" << lrn  << "|\n";

		foundbuff = body.find("--", len);
		if (foundbuff != std::string::npos)
			buffApp = body.substr(foundlen + 2, len);


		std::cout << "buffApp >>> " << buffApp << " " << buffApp.length() << ")\n";


		if (buffApp.length() == len)
		{
			outfile << buffApp;
			body = body.substr(foundbuff + 2);
			std::cout << "bodyin  >>>" << body << "\n";

			flag = 0;
		}
		else
		{
			body = body.substr(foundlen + 2);
			std::cout << "body    >>>" << body << "\n";
			flag = 1;
		}
		std::cout << "\n------------\n\n";
	}
	outfile.close();
	return (0);
}
