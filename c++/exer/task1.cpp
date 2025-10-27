#include <iostream>
#include "squareFunc.h"

int main()
{
    squareFunc maomao;
    double a, b, c;
    std::cout << "a = ";
    std::cin >> a;
    std::cout << "b = ";
    std::cin >> b;
    std::cout << "c = ";
    std::cin >> c;
    
    if (a != 0 || b != 0 || c != 0)
        maomao.setCoef(a, b, c);

    double x1, x2;
    std::cout << "x1 = ";
    std::cin >> x1;
    std::cout << "x2 = ";
    std::cin >> x2;
    
    int i;
    for (i = x1; i < x2; ++i){
        maomao.calculate(i);
        maomao.prinfFunc(i);
    }
}