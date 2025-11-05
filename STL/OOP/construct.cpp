#include<iostream>

class Something{
    const int a;
    int* ptr;
public:
    Something(int a):Something(a, 0){
        std::cout << "With 1 arg\n";
    }
    Something(int a, int x):a(a),ptr(new int(x)){
        std::cout << a << ' ' << *ptr << '\n';
    }
    Something():a(11){}
    void print() const{
        std::cout << a << '\n';
    }
    ~Something(){
        std::cout << "Cleared\n";
        delete ptr;
    }
};

int main(){
    Something obj(12);
    obj.print();
}