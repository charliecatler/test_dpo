
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

class BankAccount {
public:
    std::mutex mtx;
    int balance;
    int id;
    
public:

    BankAccount(int account_id, int initial_balance) 
        : id(account_id), balance(initial_balance) {}
    
    void transfer_to(BankAccount& other, int amount) {
        std::cout << "Account " << id << " transferring " << amount 
                  << " to account " << other.id << std::endl;
                  
        std::lock_guard<std::mutex> lock1(mtx);  // Блокируем свой mutex
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock2(other.mtx);  // Блокируем чужой
        
        if (balance >= amount) {
            balance -= amount;
            other.balance += amount;
            std::cout << "Transfer completed: " << id << " -> " << other.id << std::endl;
        }
    }
    
    int get_balance() {
        std::lock_guard<std::mutex> lock1(mtx);
        return balance;
    }
    
    int get_id() const { return id; }

};

void transfer_worker(BankAccount& from, BankAccount& to, int amount, int iterations) {
    for (int i = 0; i < iterations; i++) {
        from.transfer_to(to, amount);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {

    BankAccount account1(1, 1000);
    BankAccount account2(2, 1000);
    
    // Потоки переводят деньги друг другу - потенциальный deadlock
    std::thread t1(transfer_worker, std::ref(account1), std::ref(account2), 50, 5);
    std::thread t2(transfer_worker, std::ref(account2), std::ref(account1), 30, 5);
    
    t1.join();
    t2.join();
    
    return 0;
}