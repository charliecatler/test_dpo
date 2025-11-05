#include<iostream>
#include<string>

class Base{
public:
    void print(int x){
        std::cout << x << '\n';
    }
    void print(char c){
        std::cout << c << '\n';
    }
};

class Deriv: public Base{
    
public:
    using Base::print;
    void print(std::string str){
        std::cout << str << '\n';
    }
};

int main(){
    Deriv obj;
    obj.Base::print(111);
    obj.print('R');
    obj.print("Hello");
}