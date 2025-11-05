#include<iostream>
#include<vector>

int main(){
    std::vector vec = {'a','b','c'};
    std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    vec.push_back('d');
    std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    // vec.clear();
    // std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    // std::cout << vec[0] << ' ' << vec[1] << ' ' << vec[2] << '\n';
    vec.shrink_to_fit();
    std::cout << vec.size() << ' ' << vec.capacity() << '\n';
    auto it1 = vec.begin();
    std::cout << *it1 << '\n';
    //it1 += 2;
    std::cout << it1[2] << '\n';
}