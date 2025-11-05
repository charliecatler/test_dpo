#include <iostream>

class Secret{
    int *ptr;
    public: 
    Secret(int value):ptr(new int (value)){}
    ~Secret()
    {
        std::cout << "Clear value " << *ptr << std::endl; 
        delete ptr;
    }
    int& get(){
        return *ptr;
    }
    Secret(const Secret &obj){
        std::cout << "Copy construct " << *obj.ptr << std::endl; 
        ptr = new int(*obj.ptr);
    }
    Secret &operator = (const Secret &obj)
    {
        std::cout << "Copy operator " << *ptr << std::endl; 
        Secret temp(obj);
        std::swap(ptr, temp.ptr);
        return *this;
    }
    Secret(Secret&& obj):ptr(nullptr){
        std::swap(ptr,obj.ptr);
        // ptr = obj.ptr;
        // obj.ptr = nullptr;
    }
    Secret &operator = (Secret &obj)
    {
        std::swap(ptr,obj.ptr);
        return *this;
    }
};

int main(){
    Secret s(42);
    Secret r = s;
    s.get() = 111;
    std::cout <<(r.get()) << std::endl;
    std::cout << "Origin " << (r.get()) << std::endl;
    std::cout << "Copy " << (s.get()) << std::endl;
}