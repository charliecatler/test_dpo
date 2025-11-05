#include<iostream>
#include<map>
#include<string>

int main(){
    std::map<std::string, int> dict;
    int n;
    std::cin >> n;
    std::string key;
    for(int i = 0; i < n; ++i){
        std::cin >> key;
        dict[key] += 1;
    }
    for(auto elem: dict){
        std::cout << "Word " << elem.first <<
                     " count " << elem.second << '\n';
    }
}