
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
#include <sstream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib> 
#include <iostream> 
#include <time.h> 

int main()
{
    char buf[PATH_MAX];
    char bufR[PATH_MAX];
    char *res = realpath("./", buf);
    char *resR = realpath("./..", bufR);
	std::string s = buf;
	std::string st = bufR;
    if (res)
	{
		if (s.find(st) != std::string::npos)
		{
			printf("This source is at %s.\n", buf);
			printf("This source is at %s.\n", bufR);
		}
		else
		{
			printf("This source is at %s.\n", buf);
			printf("This source is at %s.\n", bufR);
			std::cout << "not found\n"; 
		}
    }
	else
	{
        char* errStr = strerror(errno);
        printf("error string: %s\n", errStr);

        perror("realpath");
        exit(EXIT_FAILURE);
	}

	return (0);
}
