#include <future>
#include <iostream>
#include <vector>
#include <numeric>

// Функция для параллельного суммирования массива
std::future<long long> async_sum(const std::vector<int>& data, size_t start, size_t end) {
    return std::async(std::launch::async, [&data, start, end]() {
        return std::accumulate(data.begin() + start, data.begin() + end, 0LL);
    });
}

int main() {
    std::vector<int> numbers(1024);
    std::iota(numbers.begin(), numbers.end(), 1);

    const size_t num_threads = 4;
    const size_t chunk_size = numbers.size() / num_threads;

    std::vector<std::future<long long>> futures;
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? numbers.size() : (i + 1) * chunk_size;
        futures.push_back(async_sum(numbers, start, end));
    }
    long long total_sum = 0;
    for (auto& future : futures) {
        total_sum += future.get();
    }
    std::cout << "Total sum: " << total_sum << std::endl;
    return 0;
}
