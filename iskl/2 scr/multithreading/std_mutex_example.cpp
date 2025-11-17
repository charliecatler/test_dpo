#include <thread>
#include <mutex>
#include <iostream>

//{mutex + shared_resource}
std::mutex mtx;
int shared_resource = 0;

void worker_raw_lock() {
    mtx.lock();
    shared_resource++;
    std::cout << "shared_resource: " << shared_resource << std::endl;
    mtx.unlock();
}


void worker_raii_lock() {
    std::lock_guard<std::mutex> lock(mtx);
    shared_resource++;
    std::cout << "shared_resource: " << shared_resource << std::endl;
}

int main() {
    std::thread t1(worker_raw_lock);
    std::thread t2(worker_raii_lock);
    t1.join();
    t2.join();
    return 0;
}
