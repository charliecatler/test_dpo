#include <iostream>

class Smth
{
    const int a;

public:
    Smth(int a):a(a)
    {
    }
    void print() const{
        std::cout << a << '\n';
    }
};

int main()
{
    Smth obj(10);
    obj.print();
}