#include<iostream>
#include<vector>
#include<algorithm>

int func(int x){
    return x + 1;
}

int main(){
    int n = 10;
    int* arr = new int[n];
    for(int i = 0; i < n; ++i){
        arr[i] = i*2;
    }

    auto it = std::find(arr, arr+n, 10);
    *it = 101;

    std::vector<int> vec;
    std::transform(arr, arr+n, 
            std::back_inserter(vec),
            func);

    for(auto elem: vec){
        std::cout << elem << ' ';
    }

    

}