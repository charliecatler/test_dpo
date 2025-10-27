#include <iostream>
#include "sum.h"

#include <exception>
int test(int x, int y)
{
    throw 20;
}


int main()
{
#if 0
    int x,y; 
    std::cin >> x >> y; // mojno vvodit cherez probel
    // std::cout << sum(x,y) << std::endl;
    std::cout << mult(x,y) << std::endl;
    return 0;
#endif


}