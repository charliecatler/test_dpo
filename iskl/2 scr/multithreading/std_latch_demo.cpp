#include <latch>
#include <thread>
#include <vector>
#include <iostream>

void worker_with_latch(std::latch& done_latch, int worker_id) {
    std::cout << "Worker " << worker_id << " working..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1 + worker_id % 3));
    std::cout << "Worker " << worker_id << " done" << std::endl;
    done_latch.count_down();
}

int main() {
    const int num_workers = 5;
    std::latch all_done(num_workers);
    std::vector<std::thread> workers;
    for (int i = 0; i < num_workers; ++i) {
        workers.emplace_back(worker_with_latch, std::ref(all_done), i);
    }
    all_done.wait();
    std::cout << "All workers completed!" << std::endl;
    for (auto& worker : workers) {
        worker.join();
    }
    return 0;
}
