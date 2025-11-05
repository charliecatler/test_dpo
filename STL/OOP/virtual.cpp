#include<iostream>

class Base{
public:
    virtual double op(double x, double y){
        return x + y;
    }
    virtual ~Base(){
        std::cout << "DBase\n";
    }
};

class Deriv: public Base{
public:
    double op(double x, double y){
        return x * y;
    }
    ~Deriv(){
        std::cout << "DDeriv\n";
    }
};

int main(){
    bool flag;
    std::cin >> flag;
    Base* obj;
    if(flag){
        obj = new Base;
    }
    else{
        obj = new Deriv;
    }

    std::cout << (obj->op(2.5, 10)) << '\n';
    delete obj;
}