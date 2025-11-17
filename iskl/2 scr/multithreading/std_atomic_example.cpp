#include <atomic>
#include <iostream>

int main() {
    std::atomic<int> atomic_int{0};
    atomic_int.store(42);
    std::cout << "Atomic value: " << atomic_int.load() << std::endl;
    int old_value = atomic_int.exchange(10);
    std::cout << "Old value: " << old_value << " New value: " << atomic_int.load() << std::endl;
    int expected = 10;
    if (atomic_int.compare_exchange_weak(expected, 20)) {
        std::cout << "Value changed to 20" << std::endl;
    }
    atomic_int.fetch_add(5);
    std::cout << "After fetch_add: " << atomic_int.load() << std::endl;
    ++atomic_int;
    std::cout << "After increment: " << atomic_int.load() << std::endl;
    return 0;
}
