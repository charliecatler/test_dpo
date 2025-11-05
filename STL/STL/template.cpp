#include<iostream>

template<typename T>
class MyArr{
    T* arr;
    int n;
public:
    MyArr(int n):n(n){
        arr = new T[n];
    }
    ~MyArr(){
        delete arr;
    }
    int size(){
        return n;
    }
    T& get(int i){
        return arr[i];
    }
};

struct Smth{
    int a;
    double b;
    friend std::ostream& operator << (std::ostream& out, Smth& obj){
        out << '{' << obj.a << ':' << obj.b << '}';
        return out;
    }
};

template<typename T>
void print(T obj){
    T();
    std::cout << obj << '\n';
}

int main(){
    MyArr<char> arr1(5);
    for(int i = 0; i < arr1.size(); ++i){
        arr1.get(i) = 'A' + i;
    }
    MyArr<Smth> arr2(4);
    Smth obj;
    obj.a = 10;
    obj.b = 2.5;
    print(10);
    print("Hello World!");
    print(obj);
}