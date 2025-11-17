#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>

// Потокобезопасная очередь
class ThreadSafeQueue {
    std::queue<int> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    //TODO:: добавить ограничение на размер очереди
public:

    //TODO:: добавить конструктор с инициализацией размера очереди

    void push(int item) {
        //TODO:: метод должен ожидать, если в очереди не осталось места
        // аналогично как pop в случае пустой очереди
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
        condition_.notify_one();
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty(); });
        int result = queue_.front();
        queue_.pop();
        return result;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
};

int main() {
    ThreadSafeQueue q;

    //TODO:: добавить несколько потоков производителей и потребителей

    std::thread producer([&q]{
        for (int i = 0; i < 10; ++i) {
            q.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    std::thread consumer([&q]{
        for (int i = 0; i < 10; ++i) {
            int value = q.pop();
            std::cout << "Consumed: " << value << std::endl;
        }
    });

    producer.join();
    consumer.join();
    
    return 0;
}
