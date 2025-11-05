#include<iostream>
#include<algorithm>
#include<vector>


int main(){
    std::vector vec = {1,2,3,2,4,2,5};
    std::cout << "Size before " << vec.size() << '\n';
    auto it_nend = std::remove(vec.begin(), vec.end(), 2);
    vec.erase(it_nend, vec.end());
    std::cout << "Size after " << vec.size() << '\n';
    for(auto elem: vec){
        std::cout << elem << ' ';
    }

}