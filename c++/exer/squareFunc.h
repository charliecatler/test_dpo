#include <iostream>

class squareFunc
{
public:
    double a = 1;
    double b = -2;
    double c = 1;

    void setCoef(double a, double b, double c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    double calculate(double x)
    {
        double y;
        y = a * x * x + b * x + c;
        return y;
    }
    void prinfFunc(double x){
        std::cout << a << " * " << x*x <<" + " << b << " * " << x << " + " << c << " = " << (a * x * x + b * x + c) << std::endl;
    };

};
