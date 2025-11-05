#include<iostream>
#include<algorithm>
#include<iterator>
#include<vector>
#include<fstream>

bool isAbsLess10(int x){
    return x < 10 && x > -10;
}

int main(){
    std::vector<int> vec;
    std::copy(std::istream_iterator<int>(std::cin),
              std::istream_iterator<int>(),
              std::back_inserter(vec));
    std::for_each(vec.begin(), vec.end(),
                  [](int x){std::cout << x << ' ';});
    std::ofstream fout("out_copy", std::ios_base::app);

    std::copy_if(vec.begin()+1, vec.end()-1,
                 std::ostream_iterator<int>(fout, ","),
                isAbsLess10);
}