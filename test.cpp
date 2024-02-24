#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

int main() {
    // Array of strings
    std::string methods[] = {"GET", "POST", "DELETE"};
    const size_t numMethods = sizeof(methods) / sizeof(methods[0]);
    std::vector<char> characters;
    characters.push_back('A');
    characters.push_back('B');
    characters.push_back('C');

    // String to search for
    std::string target("POST");
    // std::cout << "-----> " << numMethods << std::endl;
    // // Use std::find to search for the target string in the array
    // bool found = std::find(methods, methods + numMethods, target) != methods + numMethods;

    // // Check if the string was found
    // if (found) {
    //     std::cout << "String found" << std::endl;
    // } else {
    //     std::cout << "String not found" << std::endl;
    // }
    std::transform(target.begin(), target.end(), target.begin(),tolower);
    std::cout << "target : " << target << std::endl;
    return 0;
}
