#include <barrier>
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

void phase_worker(std::barrier<>& sync_point, int worker_id) {
    for (int phase = 0; phase < 3; ++phase) {
        std::cout << "Worker " << worker_id << " phase " << phase << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + worker_id * 100));
        sync_point.arrive_and_wait();
        std::cout << "Worker " << worker_id << " finished phase " << phase << std::endl;
    }
}

int main() {
    const int num_workers = 4;
    std::barrier sync_point(num_workers);
    std::vector<std::thread> workers;
    for (int i = 0; i < num_workers; ++i) {
        workers.emplace_back(phase_worker, std::ref(sync_point), i);
    }
    for (auto& worker : workers) {
        worker.join();
    }
    return 0;
}
