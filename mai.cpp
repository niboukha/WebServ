
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



class A
{
   public:
    int b;
      A ()
      {
        std::cout << "default constructor\n";
      }

    A(int& a) : b(a)
    {
        std::cout << "param" << std::endl;
    }
    A (const A &copy) : b(copy.b)
    {
       std::cout << "copy constructor\n";
    }
    ~A (){
        std::cout << "distructor\n";

    }

      A & operator = (const A &a)
      {
        std::cout << "assg constructor\n";
        return (*this);
      }
};

int main()
{

    std::map<int, A> mp;
    int k = 1;
    mp.insert(std::make_pair(1, A(k)));
    std::cout << "here" << std::endl;
    A obj(k);
    mp[1] = obj;
    return (0);
}
