#include <fstream>
#include <iostream>
#include <sstream>

int main()
{
    std::istringstream str("1234567");
    std::istreambuf_iterator<char> iter1{str};
    std::istreambuf_iterator<char> iter2(iter1);
    std::cout << *(iter1++) << std::endl;
    std::cout << *(iter2++) << std::endl;
}