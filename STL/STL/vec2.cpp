#include<iostream>
#include<vector>
#include<list>

int main(){
    std::list<int> vec;
    int n;
    std::cin >> n;
    vec.resize(n);
    for(auto& elem: vec){
        std::cin >> elem;
    }

    for(auto it = vec.rbegin(); it != vec.rend(); ++it){
        std::cout << *it << ' ';
    }
}