// depends
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/has_key.hpp>
#include <iostream>
namespace mpl = boost::mpl;
typedef mpl::vector<int, double, char, bool> my_types;


int main() {
    std::cout << "Number of types: " << mpl::size<my_types>::value << std::endl;

    // Попытка найти тип float внутри my_types
    typedef mpl::find<my_types, float>::type iter;
    typedef mpl::end<my_types>::type end_iter;

    // Проверка, найден ли тип float
    bool has_float = !std::is_same<iter, end_iter>::value;
    std::cout << "Contains float? " << (has_float ? "Yes" : "No") << std::endl;

    typedef mpl::find<my_types, int>::type iter_int; // Аналогично для int
    bool has_int = !std::is_same<iter_int, end_iter>::value;
    std::cout << "Contains int? " << (has_int ? "Yes" : "No") << std::endl;

    return 0;

}
