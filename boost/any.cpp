#include <iostream>
#include <vector>
#include <string>
#include <boost/any.hpp>

int main()
{
    std::vector<boost::any> collection;

    collection.emplace_back(42);
    collection.emplace_back("ShiNi");
    collection.emplace_back(42.42f);
    collection.emplace_back(std::string("ShiNiGaMi"));

    for (const auto &item : collection)
    {
        if (item.type() == typeid(int))
            std::cout << "int: " << boost::any_cast<int>(item) << std::endl;
        else if (item.type() == typeid(float))
            std::cout << "float: " << boost::any_cast<float>(item) << std::endl;
        else if (item.type() == typeid(std::string))
            std::cout << "std::string: " << boost::any_cast<std::string>(item) << std::endl;
        else if (item.type() == typeid(const char *))
            std::cout << "const char *: " << boost::any_cast<const char *>(item) << std::endl;
    }
    return 0;
}