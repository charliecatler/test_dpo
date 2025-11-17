#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cassert>

class Counter {
private:
    int count; //TODO:: make it atomic
    int iters;
public:
    Counter(int num_iters) : count(0), iters(num_iters) {}
    
    void inc() { //TODO:: fix increment to atomic
        for (int i = 0; i < iters; i++) {
            count++; 

            //delay timeout for data race showing
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    int get_count() const { return count; }
    int get_iters() const { return iters; }
};

void worker(Counter& counter, int thread_id) {
    std::cout << "Thread " << thread_id << " starting..." << std::endl;
    counter.inc();
    std::cout << "Thread " << thread_id << " finished" << std::endl;
}

int main() {
    Counter counter(100);
    std::vector<std::thread> threads;
    size_t number_of_threads = std::thread::hardware_concurrency(); // get available number of hardware threads here
    std::cout<<"Number of hardware threads: "<< number_of_threads << std::endl;

    for (size_t i = 0; i < number_of_threads; i++) {
        threads.emplace_back(worker, std::ref(counter), (int)i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    int expected_value = number_of_threads*counter.get_iters();

    std::cout << "Final count: " << counter.get_count() << std::endl;
    std::cout << "Expected:"<< expected_value << std::endl;
    
    assert(counter.get_count()== expected_value);

    return 0;
}