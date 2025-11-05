#include<iostream>

class Counter{
    static int count;
public:
    int sum(int x, int y){
        count++;
        return x + y;
    }
    static int getCount(){
        return count;
    }
};

int Counter::count = 0;

int main(){
    Counter c1;
    Counter c2;
    for(int i = -5; i <= 5; ++i){
        std::cout << (c1.sum(i,i)) << ' ';
    }
    std::cout << "\nOld count is " << Counter::getCount() << '\n';
    c2.sum(1,2);
    std::cout << "\nCount is " << c2.getCount() << '\n';
}