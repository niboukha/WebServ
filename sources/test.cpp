#include"../includes/WebServ.hpp"

int main()
{
    std::list<int> my_list;
auto it = my_list.begin();
//  // now points to the second element
// it = my_list.erase(it);

// it = my_list.begin();
// std::cout << "------> " << *it << std::endl;
// // erase the second element and get an iterator to the next element
// // ++it;
// it = my_list.erase(it);
// // ++it;
// it = my_list.erase(it);
// // ++it;
// it = my_list.erase(it);
// // ++it;
// it = my_list.begin();
// // std::cout << "===> " << *it << std::endl;
// it = my_list.erase(it);
// std::cout << my_list.size() << "\n";
my_list.push_back(6); 
it = my_list.erase(it);
// push_back to the list safely
// auto it = my_list.begin();
std::cout << "===> " << *it << std::endl;

}