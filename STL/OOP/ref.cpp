#include<iostream>

void func(int& value){
    std::cout << "Reference lvalue\n";
}

void func(int&& value){
    std::cout << "Reference rvalue\n";
}

void func(const int& value){
    std::cout << "Const Reference lvalue\n";
}

int main(){
    int x = 10;
    const int y = 15;
    func(x);
    func(5);
    func(y);
}