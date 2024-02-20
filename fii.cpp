#include <iostream> 
#include <stdio.h>
int main()
{
    const char* s = "<body>\n<div>\n<h1>204 No Content : %s </h1>\n<p>The requested page could not be found.</p>\n</div></body></html>";

    
    std::string str;

    std::cin >> str;
    printf(s, str.c_str());
    
    return (0);
}