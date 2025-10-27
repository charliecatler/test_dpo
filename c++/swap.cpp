#include <iostream>

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void func(const int a){

}

int main(){
    int k = 10, m = 20;
    swap(&k,&m);
    std::cout << "k: " << k << " m: " << m << "\n";
    int* ptr = nullptr; //NULL //0 

    func(NULL);
    func(nullptr);
}