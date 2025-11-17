#include <iostream>
#include <stdexcept>
#include <string>

class Demo {
    std::string name_;
public:
    Demo(const std::string& name) : name_(name) {
        std::cout << "Constructor: " << name_ << std::endl;
    }
    
    ~Demo() {
        std::cout << "Destructor: " << name_ << std::endl;
    }
    
    void do_work() {
        std::cout << "Working: " << name_ << std::endl;
    }
};

void function_C() {
    Demo obj_c("Object_C");
    obj_c.do_work();
    
    throw std::runtime_error("Error in function C");
    
    std::cout << "After exception in C" << std::endl;
}

void function_B() {
    Demo obj_b("Object_B");  
    obj_b.do_work();
    
    {
        Demo obj_b_inner("Object_B_inner");
        function_C();
    }
    
    std::cout << "After call to C in B" << std::endl;
}

void function_A() {
    Demo obj_a("Object_A");
    obj_a.do_work();
    
    try {
        function_B();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    std::cout << "Continuing work in A" << std::endl;
}

int main() {
    std::cout << "=== Stack Unwinding Demonstration ===" << std::endl;
    function_A();
    std::cout << "=== End of program ===" << std::endl;
    return 0;
}