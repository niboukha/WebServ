#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
  string arrey;
  arrey = "		hello";
  if (arrey.find("\t") != std::string::npos) {
      std::cout << "found a tab!" << '\n';
  }
  return 0;
}