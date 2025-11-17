#include <iostream>
#include <vector>
#include <cstring>  // для memcpy

int main() {

    //STL container
    std::vector<int> numbers_vector = {11, 22, 33, 44, 55};
    size_t vector_len = numbers_vector.size();

    // C-style array
    int numbers_array[vector_len];
    // Копируем данные из вектора в массив
    std::memcpy(numbers_array, numbers_vector.data(), vector_len*sizeof(int));
    
    // Выводим значения
    std::cout << "i" << "\t" << "Vec[i]" << "\t" << "Arr[i]"  << std::endl;
    for (int i = 0; i <= vector_len; i++) {
        std::cout << "[" << i << "]" << 
                                    "\t" << numbers_vector[i] <<
                                    "\t" << numbers_array[i]  <<std::endl;
    }
    
    return 0;
}