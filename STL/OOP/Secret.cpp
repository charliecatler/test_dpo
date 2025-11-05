#include<iostream>

class Secret{
    int* ptr;
public:
    Secret(int value):ptr(new int(value)){}
    ~Secret(){
        std::cout << "Clear value " << *ptr << '\n';
        delete ptr;
    }
    int& get(){
        return *ptr;
    }
    Secret(const Secret& obj){
        std::cout << "Copy construct " << *obj.ptr << '\n';
        ptr = new int(*obj.ptr);
    }
    Secret& operator = (const Secret& obj){
        std::cout << "Copy operator from " << *ptr << " to "<< *obj.ptr << '\n';
        Secret temp(obj);
        std::swap(ptr, temp.ptr);
        return *this;
    }
    Secret(Secret&& obj):ptr(nullptr){
        std::swap(ptr, obj.ptr);
        // ptr = obj.ptr;
        // obj.ptr = nullptr;
    }
    Secret& operator = (Secret&& obj){
       std::swap(ptr, obj.ptr);
       return *this; 
    }
};

int main(){
    Secret s(42);
    Secret r(10);
    r = std::move(s);
    s.get() = 111;
    std::cout << "Origin " << (s.get()) << '\n';
    std::cout << "Copy   " << (r.get()) << '\n';
}