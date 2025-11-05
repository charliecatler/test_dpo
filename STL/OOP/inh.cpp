#include<iostream>

class Base{
public:
    void f(){
        std::cout << "Base\n";
    }
};

class Deriv: public Base{
public:
    void f(){
        std::cout << "Deriv\n";
    }
};

class Final: public Deriv{
public:
};

int main(){
    Final obj;
    obj.Base::f();
}