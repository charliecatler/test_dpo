#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

template<typename T>
class SafeContainer {
    T* data_;
    size_t size_;
    size_t capacity_;
    
    T* safe_copy(const T* source, size_t count) {
        if (count == 0) return nullptr;
        
        T* dest = new T[count];
        
        try {
            for (size_t i = 0; i < count; ++i) {
                dest[i] = source[i];
            }
        } catch (...) {
            delete[] dest;
            throw;
        }
        
        return dest;
    }
    
public:
    SafeContainer() : data_(nullptr), size_(0), capacity_(0) {}
    
    explicit SafeContainer(size_t capacity) : capacity_(capacity), size_(0) {
        data_ = new T[capacity_];
    }
    
    // Copy constructor
    SafeContainer(const SafeContainer& other) 
        : data_(safe_copy(other.data_, other.capacity_))
        , size_(other.size_)
        , capacity_(other.capacity_) {}
    
    // Copy assignment через copy-and-swap
    SafeContainer& operator=(const SafeContainer& other) {
        if (this != &other) {
            SafeContainer temp(other);
            swap(temp);
        }
        return *this;
    }
    
    // Move constructor (noexcept)
    SafeContainer(SafeContainer&& other) noexcept 
        : data_(other.data_)
        , size_(other.size_)
        , capacity_(other.capacity_) {
        
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    // Move assignment
    SafeContainer& operator=(SafeContainer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    ~SafeContainer() {
        delete[] data_;
    }
    
    void swap(SafeContainer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            throw std::overflow_error("Container is full");
        }
        data_[size_++] = value;
    }
    
    T& operator[](size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }
    
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
};

template<typename T>
void swap(SafeContainer<T>& a, SafeContainer<T>& b) noexcept {
    a.swap(b);
}

int main() {
    std::cout << "=== Copy-and-Swap Pattern Example ===" << std::endl;
    
    try {
        SafeContainer<int> container1(10);
        
        container1.push_back(1);
        container1.push_back(2);
        container1.push_back(3);
        
        std::cout << "Container1 size: " << container1.size() << std::endl;
        
        // Copy assignment - использует copy-and-swap
        SafeContainer<int> container2(20);
        container2 = container1;
        
        std::cout << "Container2 size: " << container2.size() << std::endl;
        std::cout << "Container2[0]: " << container2[0] << std::endl;
        
        // Move assignment - быстрое перемещение
        SafeContainer<int> container3 = std::move(container2);
        std::cout << "Container3 size: " << container3.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
