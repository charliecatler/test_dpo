# Часть 3: Мастер-класс по отладке программ с использованием GDB

### 🎯 Цели части 3

- **Освоить специфику отладки C++** по сравнению с привычной отладкой C
- **Изучить отладку исключений** и stack unwinding механизма
- **Овладеть многопоточной отладкой** с современными примитивами C++
- **Получить практические навыки** решения реальных проблем

---
#### **Техническая проверка окружения**
```bash
# Проверяем наличие всех файлов
$ ls -la
task1.cpp task2.cpp task3.cpp task4.cpp Makefile

# Компилируем все задания
$ make all
g++ -std=c++17 -g -O0 -pthread -Wall -Wextra -o task1 task1.cpp
g++ -std=c++17 -g -O0 -pthread -Wall -Wextra -o task2 task2.cpp
g++ -std=c++17 -g -O0 -pthread -Wall -Wextra -o task3 task3.cpp
g++ -std=c++17 -g -O0 -pthread -Wall -Wextra -o task4 task4.cpp

# Проверяем версию GDB
$ gdb --version
GNU gdb (GDB) 10.2 # Должна быть 8+
```

#### **Краткий обзор отличий C++ от C в отладке**
- **STL контейнеры:** `print vector.size()` работает в GDB
- **Исключения:** специальные команды `catch throw/catch`
- **Многопоточность:** `std::thread` вместо `pthread_t`
- **RAII объекты:** автоматические деструкторы

---

### 📋**3.1 Отладка STL и базовых ошибок**

#### **Практическое задание: Buffer overflow в std::vector**

**Код для отладки (task1.cpp):**
```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Намеренная ошибка: выход за границы
    for (int i = 0; i <= numbers.size(); i++) {  // BUG: <=
        std::cout << "Number[" << i << "] = " << numbers[i] << std::endl;
    }
    
    return 0;
}
```

**Пошаговая отладка с объяснениями:**

**Шаг 1: Запуск и первая ошибка**
```bash
$ gdb ./task1
(gdb) run

# Ожидаемый вывод:
Number[0] = 1
Number[1] = 2
Number[2] = 3
Number[3] = 4
Number[4] = 5
Number[5] = 32767  # Мусорное значение или segfault
```

**💡 Пояснение:**
```
В C мы бы увидели аналогичную проблему с массивами. Но в C++ 
std::vector предоставляет больше информации для отладки.
Давайте посмотрим, что GDB может рассказать о векторе.
```

**Шаг 2: Анализ состояния программы**
```bash
(gdb) bt
#0  main () at task1.cpp:8

(gdb) print i
$1 = 5

(gdb) print numbers
$2 = std::vector of length 5, capacity 5 = {1, 2, 3, 4, 5}

(gdb) print numbers.size()
$3 = 5
```

**📚 Детальное объяснение:**
```
Видите разницу с C? GDB показывает не просто адрес, а содержимое 
вектора с его length и capacity. Это называется 'pretty printing'.
Мы сразу видим: i=5, size()=5, значит обращение к [5] некорректно.
```

**Шаг 3: Пошаговая отладка цикла**
```bash
(gdb) break task1.cpp:7
Breakpoint 1 at 0x4011a8: file task1.cpp, line 7.

(gdb) run
Breakpoint 1, main () at task1.cpp:7

(gdb) print i
$4 = 0

(gdb) continue  # Проходим несколько итераций
(gdb) print i
$5 = 1

# Продолжаем до проблемной итерации
(gdb) continue
(gdb) continue
(gdb) continue
(gdb) continue
(gdb) print i
$6 = 4  # Последняя валидная итерация

(gdb) continue
(gdb) print i
$7 = 5  # Проблемная итерация!
```

**🔧 Исправление и проверка:**
```cpp
// Исправленная версия
for (int i = 0; i < numbers.size(); i++) {  // Исправлено: <
```

---

### 📋**3.2 Отладка исключений**

#### **Практическое задание: Исключения и утечки памяти**

**Код для отладки (task2.cpp):**
```cpp
#include <iostream>
#include <stdexcept>

class Resource {
private:
    int* data;
    size_t size;
    
public:
    Resource(size_t s) : size(s) {
        std::cout << "Creating resource of size " << s << std::endl;
        data = new int[s];  // Потенциальная утечка
        
        for (size_t i = 0; i < s; i++) {
            data[i] = i * 2;
        }
        
        if (s > 1000) {
            throw std::runtime_error("Resource too large!");
        }
    }
    
    ~Resource() {
        std::cout << "Destroying resource" << std::endl;
        delete[] data;
    }
    
    void process() {
        std::cout << "Processing resource..." << std::endl;
        int result = 100 / (size - size);  // Division by zero!
    }
};

void dangerous_function() {
    Resource* res = new Resource(500);
    
    try {
        res->process();
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return;  // Утечка памяти!
    }
    
    delete res;
}

int main() {
    try {
        dangerous_function();
        Resource big_resource(2000);  // Исключение в конструкторе
    } catch (const std::exception& e) {
        std::cout << "Main caught: " << e.what() << std::endl;
    }
    
    return 0;
}
```

**Пошаговая отладка исключений:**

**Шаг 1: Настройка перехвата исключений**
```bash
$ gdb ./task3
(gdb) catch throw
Catchpoint 1 (throw)

(gdb) catch catch
Catchpoint 2 (catch)

(gdb) info catch
Num     Type            What
1       throw           exception throw
2       catch           exception catch
```

**💡 Объяснение команд:**
```
catch throw - это специальная команда GDB для C++, которой не было в C.
Она останавливает выполнение в момент выброса любого исключения,
до начала stack unwinding'а.
```

**Шаг 2: Отлов первого исключения (division by zero)**
```bash
(gdb) run

# Вывод программы:
Creating resource of size 500
Processing resource...

# GDB останавливается:
Catchpoint 1 (exception thrown), 0x... in __cxa_throw ()

(gdb) bt
#0  0x... in __cxa_throw ()
#1  0x... in Resource::process() at task3.cpp:30
#2  0x... in dangerous_function() at task3.cpp:39
#3  0x... in main() at task3.cpp:47
```

**📚 Анализ stack trace:**
```
__cxa_throw - это внутренняя функция C++ runtime.
Стек показывает: main → dangerous_function → Resource::process → throw
В C такой детальной трассировки исключений не было бы.
```

**Шаг 3: Исследование состояния объекта**
```bash
(gdb) up 2  # Переходим в dangerous_function

(gdb) print res
$1 = (Resource *) 0x555555756280

(gdb) print *res
$2 = {data = 0x555555756290, size = 500}

(gdb) print res->data[0]
$3 = 0
(gdb) print res->data[1] 
$4 = 2
```

**⚠️ Критический момент - утечка памяти:**
```
Объект создан (data != nullptr), но после 
исключения мы делаем return без delete res. Это утечка памяти!
В C++ правильное решение - использовать RAII и умные указатели.
```

**Шаг 4: Продолжение для второго исключения**
```bash
(gdb) continue

# Вывод:
Exception caught: floating point exception
Creating resource of size 2000

# Снова останавливается на исключении:
Catchpoint 1 (exception thrown), 0x... in __cxa_throw ()

(gdb) bt
#0  0x... in __cxa_throw ()  
#1  0x... in Resource::Resource(size_t) at task3.cpp:18
#2  0x... in main() at task3.cpp:48
```

**💡 Объяснение исключения в конструкторе:**
```
Исключение в конструкторе означает, что объект 
не считается 'полностью созданным'. Деструктор НЕ ВЫЗОВЕТСЯ!
Но память уже выделена - это гарантированная утечка.
```

---

### 📋**3.3 Многопоточная отладка**

#### **Практическое задание: Race condition**

**Код для отладки (task3.cpp):**
```cpp
#include <iostream>
#include <thread>
#include <vector>

class Counter {
private:
    int count;
    // std::mutex mtx;  // Намеренно закомментирован
    
public:
    Counter() : count(0) {}
    
    void increment() {
        for (int i = 0; i < 1000; i++) {
            count++;  // Race condition!
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
    
    for (int i = 0; i < 4; i++) {
        threads.emplace_back(worker, std::ref(counter), i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final count: " << counter.get_count() << std::endl;
    std::cout << "Expected: 4000" << std::endl;
    
    return 0;
}
```

**Отладка многопоточности:**

**Шаг 1: Запуск и наблюдение проблемы**
```bash
$ gdb ./task2
(gdb) run

# Возможный вывод (результат может варьироваться):
Thread 0 starting...
Thread 1 starting...
Thread 2 starting...
Thread 3 starting...
Thread 1 finished
Thread 0 finished
Thread 3 finished
Thread 2 finished
Final count: 3847  # Меньше ожидаемого 4000!
Expected: 4000
```

**💡 Объяснение race condition:**
```
Каждый поток должен добавить 1000, итого 4000. Но результат меньше.
Это классический race condition - операция count++ не атомарна!
В C с pthread'ами мы бы видели ту же проблему, но с другими инструментами отладки.
```

**Шаг 2: Анализ потоков во время выполнения**
```bash
(gdb) run
# Во время выполнения нажимаем Ctrl+C

^C
Program received signal SIGINT, Interrupt.

(gdb) info threads
  Id   Target Id         Frame
* 1    Thread 0x7ffff7fc0740 (LWP 12345) "task2" main () at task2.cpp:35
  2    Thread 0x7ffff77bf700 (LWP 12346) "task2" worker () at task2.cpp:18  
  3    Thread 0x7ffff6fbe700 (LWP 12347) "task2" Counter::increment() at task2.cpp:12
  4    Thread 0x7ffff67bd700 (LWP 12348) "task2" Counter::increment() at task2.cpp:12
  5    Thread 0x7ffff5fbc700 (LWP 12349) "task2" worker () at task2.cpp:19
```

**📚 Детальное объяснение info threads:**
```
"* - текущий активный поток (main)
LWP - Linux Lightweight Process ID
Frame - где сейчас находится каждый поток

Видите потоки 3 и 4 находятся в Counter::increment() - 
они одновременно модифицируют count!"
```

**Шаг 3: Переключение между потоками**
```bash
(gdb) thread 3
[Switching to thread 3 (Thread 0x7ffff6fbe700 (LWP 12347))]

(gdb) print count
$1 = 2847

(gdb) print i
$2 = 654  # Текущая итерация в этом потоке

(gdb) thread 4
[Switching to thread 4 (Thread 0x7ffff67bd700 (LWP 12348))]

(gdb) print count  
$3 = 2851  # Другое значение!

(gdb) print i
$4 = 723
```

**⚠️ Визуализация race condition:**
```
Значения count разные в разных потоках (2847 vs 2851)!
Это происходит потому что они читают-модифицируют-записывают
одновременно. Классический lost update problem.
```

**Шаг 4: Анализ всех потоков одновременно**
```bash
(gdb) thread apply all bt

Thread 5 (Thread 0x7ffff5fbc700 (LWP 12349)):
#0  worker () at task2.cpp:19
#1  ...

Thread 4 (Thread 0x7ffff67bd700 (LWP 12348)):  
#0  Counter::increment() at task2.cpp:12
#1  worker () at task2.cpp:18
#2  ...

Thread 3 (Thread 0x7ffff6fbe700 (LWP 12347)):
#0  Counter::increment() at task2.cpp:12  
#1  worker () at task2.cpp:18
#2  ...
```

#### **Практическое задание: Deadlock** (взаимоблокировка)

**Код для отладки (task4.cpp):**
```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

class BankAccount {
private:
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
    
    int get_balance() const {
        std::lock_guard<std::mutex> lock(mtx);
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
```

**Отладка deadlock:**

**Шаг 1: Запуск с timeout (программа зависнет)**
```bash
$ timeout 10s gdb ./task4
(gdb) run &

# Программа начнет выполняться и "зависнет"
Account 1 transferring 50 to account 2
Account 2 transferring 30 to account 1
# ... тишина
```

**Шаг 2: Прерывание и анализ**
```bash
# Нажимаем Ctrl+C в GDB
^C
Program received signal SIGINT, Interrupt.

(gdb) info threads
  Id   Target Id         Frame
* 1    Thread 0x7ffff7fc0740 (LWP 12345) "task4" main () at task4.cpp:48
  2    Thread 0x7ffff77bf700 (LWP 12346) "task4" 0x... in __lll_lock_wait ()
  3    Thread 0x7ffff6fbe700 (LWP 12347) "task4" 0x... in __lll_lock_wait ()
```

**🚨 Признаки deadlock:**
```
Потоки 2 и 3 находятся в __lll_lock_wait() - они ждут освобождения mutex'ов.
Это низкоуровневая функция блокировки. Классический признак deadlock!
```

**Шаг 3: Детальный анализ каждого потока**
```bash
(gdb) thread 2
[Switching to thread 2 (Thread 0x7ffff77bf700 (LWP 12346))]

(gdb) bt
#0  __lll_lock_wait () at lowlevellock.S:135
#1  0x... in std::mutex::lock()
#2  0x... in std::lock_guard<std::mutex>::lock_guard()
#3  0x... in BankAccount::transfer_to() at task4.cpp:22  
#4  0x... in transfer_worker() at task4.cpp:41

(gdb) up 3  # Переходим в transfer_to
(gdb) print this->id
$1 = 1  # Аккаунт 1 пытается перевести

(gdb) print &other  
$2 = (BankAccount *) 0x7fffffffe050

(gdb) thread 3
(gdb) bt
#0  __lll_lock_wait () at lowlevellock.S:135
#1  0x... in std::mutex::lock()
#2  0x... in std::lock_guard<std::mutex>::lock_guard() 
#3  0x... in BankAccount::transfer_to() at task4.cpp:22
#4  0x... in transfer_worker() at task4.cpp:41

(gdb) up 3
(gdb) print this->id
$3 = 2  # Аккаунт 2 пытается перевести

(gdb) print &other
$4 = (BankAccount *) 0x7fffffffe040
```

**🔍 Анализ взаимоблокировки:**
```
Поток 1: держит mutex аккаунта 1, ждет mutex аккаунта 2
 Поток 2: держит mutex аккаунта 2, ждет mutex аккаунта 1
 Классический взаимный deadlock!
```

---


### **Автоматизация и полезные настройки**

**Создание макросов для многопоточной отладки:**
```bash
(gdb) define threads-info
> info threads
> thread apply all bt
> end

(gdb) threads-info
# Теперь одной командой получаем полную информацию
```

**Полезные conditional breakpoints:**
```bash
# Остановиться только при определенном значении
(gdb) break task2.cpp:12 if count > 3000

# Для многопоточности - остановиться в определенном потоке
(gdb) break BankAccount::transfer_to if id == 1
```

**Настройка .gdbinit для C++:**
```bash
# ~/.gdbinit
set print pretty on
set print array on  
set print object on
set confirm off

# Макросы для C++ отладки
define print-stl-string
  printf "%s\n", $arg0.c_str()
end

define print-vector-info
  printf "size=%d, capacity=%d\n", $arg0.size(), $arg0.capacity()
end
```

---

## 📋 Практические результаты мастер-класса

### **Решённые проблемы:**

1. **Buffer overflow в std::vector** → исправлен условие цикла
2. **Race condition** → добавлен `std::mutex` с `std::lock_guard`  
3. **Memory leak при исключениях** → заменён raw pointer на `std::unique_ptr`
4. **Deadlock** → использован `std::lock()` для одновременной блокировки

### **Освоенные техники:**

- **STL debugging:** `print vector.size()`, pretty printing
- **Exception debugging:** `catch throw/catch`, анализ stack unwinding
- **Multithreading:** `info threads`, `thread apply all bt`
- **Deadlock detection:** анализ `__lll_lock_wait()` состояний

---
