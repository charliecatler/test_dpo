#include <thread>
#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

// 1. Функция без аргументов
void hello() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Hello from thread!" << std::endl;
}

// 2. Функция с параметрами, передающимися по значению
void print_number(int n) {
    std::cout << "Number: " << n << std::endl;
}

// 3. Функция с параметрами, передающимися по ссылке
//    здесь через аргумент res функция возвращает значение
void sum_vector(std::vector<int>& in_vector, int& res) {
    res = std::accumulate(in_vector.begin(), in_vector.end(), 0);
    std::cout << "Vector accumulation done,  res = " << res << std::endl;

}

// 4. Класс с operator()
class Callable {
public:
    void operator()() {
        std::cout << "Called from class!" << std::endl;
    }
};

int main() {

    std::thread t1(hello);                    // функция без аргументов

    std::thread t2(print_number, 42);         // функция с параметром

    std::vector<int> vec = {10,1,1,1,1};
    int result = 0;
    std::thread t3(sum_vector, std::ref(vec), std::ref(result)); // функция с аргументами по ссылке
   
    std::thread t4(Callable{});               // функтор

    std::thread t5([]{                        // лямбда
        std::cout << "Lambda thread!" << std::endl;
    });

    t1.join(); // TODO: попробуйте забыть вызвать join, что будет?
               // а если сделать detach() вместо join() ?
               
    t2.join();
    t3.join();
    t4.join();
    t5.join();

     std::cout << "Sum of vector = " << result << std::endl;

    return 0;
}
