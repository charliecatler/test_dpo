#include "sum.h"

int sum(int x, int y){
    return x+y;
}

int mult(int x, int y){
    if (y == 0)
        return 0;

    int m = 0;
    int i = 0;
    for (i = 0; i < y; ++i){
        m += x;
    }
    return m;
}