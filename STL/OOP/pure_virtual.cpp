#include<iostream>

class OpPrint{
public:
    void print(int x){
        std::cout << x << '\n';
    }
    virtual int op(int x, int y) = 0;
    virtual void printOpResult(int x, int y){
        print(op(x,y));
    }
};

class SumPrint: public OpPrint{
public:
    int op(int x, int y){
        return x + y;
    }
};

class ProdPrint: public OpPrint{
public:
    int op(int x, int y){
        return x * y;
    }
    void printOpResult(int x, int y){
        std::cout << "Prod Result: ";
        OpPrint::printOpResult(x,y);
    }
};

void client(OpPrint* op_obj){
    op_obj->printOpResult(4,5);
}

int main(){
    SumPrint sp;
    ProdPrint pp;
    client(&sp);
    client(&pp);
}


