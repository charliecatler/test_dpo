#include<iostream>
#include<algorithm>
#include<list>
#include<vector>
#include<iterator>

template<typename T>
void f(T begin, T end){
    std::for_each(begin, end,
                 [](int& x){x*=x;});
    std::copy(begin, end,
              std::ostream_iterator<double>(std::cout," "));
    std::cout << '\n';
}

int main(){
    std::vector vec = {1,2,3,4,5};
    std::list lst = {1,2,3,4,5};
    f(vec.begin(), vec.end());
    f(lst.begin(), lst.end());
}