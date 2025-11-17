#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cassert>

class Counter {
private:
    int count;
    
public:
    Counter() : count(0) {}
    
    void increment() {
        for (int i = 0; i < 1000; i++) {
            count++; 
            //dummy for delay 
            for (int delay = 0; delay<10000; delay++) {}
        }
    }
    
    int get_count() const { return count; }
};

void worker(Counter& counter, int thread_id) {
    std::cout << "Thread " << thread_id << " starting..." << std::endl;
    counter.increment();
    std::cout << "Thread " << thread_id << " finished" << std::endl;
}

int main() {
    Counter counter;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 8; i++) {
        threads.emplace_back(worker, std::ref(counter), i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final count: " << counter.get_count() << std::endl;
    std::cout << "Expected: 8000" << std::endl;
    
    assert(counter.get_count()==8000);

    return 0;
}