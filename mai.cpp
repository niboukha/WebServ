
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <fstream>

int main()
{
	std::ofstream	file;

	file.open("file.html");
	char	cwd[PATH_MAX];
	DIR		*pDir;
	struct dirent *pDirent;

		if (!getcwd(cwd, sizeof(cwd)))
			std::cout << "fff\n"; 
        std::cout << cwd << "\n";
		pDir = opendir(cwd);
		if (!pDir)
           	std::cout << "rrr\n"; 
		file << "<!DOCTYPE html>\n<html>\n<body>\n\n<h1>Index of/</h1>\n<hr />\n";
		while ((pDirent = readdir(pDir)))
			file <<  "<p><a href=\"" << pDirent->d_name << "\">" << pDirent->d_name << "</a></p>" "\n"; 
		file << "\n</body>\n</html>\n";
		closedir(pDir);
}
