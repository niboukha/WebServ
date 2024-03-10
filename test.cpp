#include <iostream>
#include <string>
#include <map>
#include <exception>
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
#include <csignal>
#include <cstdlib> 
#include <iostream> 
#include <time.h> 
#include <exception>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <iterator>
#include <arpa/inet.h>
#include <sstream>
#include <errno.h>
#include <sys/select.h>
#include <list>
#include <netdb.h>
#include <sys/wait.h>


int main() {
    // sleep(3);

   	char *arr[] = {(char*)"csd", NULL};


    std::cout << "CHECK CEHCK CHCEKC" << std::endl;
    execve("/nfs/homes/niboukha/Desktop/webserv/cgitest", arr, NULL );
    return 0;
}
