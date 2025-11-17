#include <iostream>
#include <stdexcept>
#include <string>

class Resource {
private:
    int* data;
    size_t size;
    
public:
    Resource(size_t res_size) : size(res_size) {
        std::cout << "Creating resource of size " << res_size << std::endl;
        data = new int[res_size]; 
        
        for (size_t i = 0; i < res_size; i++) {
            data[i] = i * 2;
        }
        
        // Исключение в конструкторе
        if (res_size > 1000) throw std::runtime_error("Resource too large!");
    }
    
    ~Resource() {
        std::cout << "Destroying resource" << std::endl;
        delete[] data;
    }
    
    void replace_data(std::string str, size_t idx) {
        std::cout << "Replacing data ..." << std::endl;
        if(idx>=size) throw std::runtime_error("Idx out of range!");
        data[idx] = std::stoi(str);
    }
};

void dangerous_function() {
    Resource* res = new Resource(500);
    
    try {
        //res->replace_data("10", 2); // без исключений
        //res->replace_data("abc", 2);  // исключение stoi
        res->replace_data("10", 501);  // исключение idx out of range

    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return; 
    }
    
    delete res;
}

int main() {
    try {
        dangerous_function();
        Resource big_resource(2000);
    } catch (const std::exception& e) {
        std::cout << "Main caught: " << e.what() << std::endl;
    }
    
    return 0;
}