#include <thread>
#include <stop_token>
#include <iostream>
#include <chrono>

void non_cancellable_task() {
    int counter = 0;
    while (true) {
        std::cout << "[NC] Working... " << ++counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (counter > 10) break;
    }
    std::cout << "Non_cancelable:: Task finished gracefully" << std::endl;
}

void cancellable_task(std::stop_token stoken) {
    int counter = 0;
    while (!stoken.stop_requested()) {
        std::cout << "[C] Working... " << ++counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (counter > 20) break;
    }
    std::cout << "Cancelable:: Task finished gracefully" << std::endl;
}

int main() {
    std::jthread jt2(non_cancellable_task);

    std::jthread jt1(cancellable_task);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    jt1.request_stop();

    return 0;
}
