#include <iostream>

#if 1
void func(int& value){
    std::cout << "ref lvalue\n"; // sleva do =
}
void func(int&& value){
    std::cout << "ref rvalue\n"; // sprava do =
}
#endif
void func(const int& value){
    std::cout << "const ref\n"; // iz-za const poh (i do =, i posle =)
}

int main(){
    int x = 10;
    const int y = 16;
    func(x); 
    func(5);
    func(y);
}