#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/algorithm.hpp>
#include <iostream>

struct Point {
    double x;
    double y;
};
// Макрос BOOST_FUSION_ADAPT_STRUCT позволяет использовать метапрограммирование FUSION
BOOST_FUSION_ADAPT_STRUCT(Point, (double, x)(double, y))

int main() {
    Point p{3.0, 4.0};

    // Используем boost::fusion::for_each для итерации по полям структур
    boost::fusion::for_each(p, [](auto& field) {
        std::cout << field << " ";
    });
    std::cout << std::endl;
}