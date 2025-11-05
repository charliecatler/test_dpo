#include<iostream>
#include<vector>
#include<algorithm>

int gen_func(){
    static int var = 0;
    var += 1;
    return var % 5;
}

int main(){
    std::vector<int> vec(30);
    std::generate(vec.begin(), vec.end(), gen_func);
    for(auto elem: vec){
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}