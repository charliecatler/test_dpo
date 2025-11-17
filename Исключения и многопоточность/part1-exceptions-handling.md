# Часть 1: Механизмы обработки исключений в С++
---
## 🎯 Цели части 1

- **Понять философию исключений** как альтернативы кодам ошибок
- **Познакомиться с механизмом раскрутки стека (stack unwinding)** и его влияние на программу
- **Изучить гарантии безопасности (exception safety)** и их практическое применение
- **Научиться мигрировать C-код** на современные C++ подходы

---
## 📋**1.1: От кодов ошибок к исключениям**

### Ошибка (Error)
Нарушение условий корректной работы программы, которое может быть обнаружено во время выполнения или компиляции.

**Классификация ошибок:**
- **Логические ошибки** - неправильный алгоритм
- **Системные ошибки** - недоступность ресурсов (файл, память, сеть)
- **Пользовательские ошибки** - некорректный ввод данных

### Обработка ошибок (Error Handling)
Процесс обнаружения, передачи информации и восстановления после ошибок в программе.

**Основные подходы:**
1. **Коды возврата** (return codes) - функция возвращает специальное значение
2. **Глобальные переменные** (errno в C) - информация об ошибке сохраняется глобально
3. **Исключения** (exceptions) - специальный механизм передачи ошибок

### Традиционных подходы к обработке ошибок на C

#### Коды возврата (Return Codes)

**Принцип:** Функция возвращает специальное значение, указывающее на успех или тип ошибки.

**Преимущества:**
- ✅ Простота реализации
- ✅ Контролируемость
- ✅ Совместимость с C
- ✅ Предсказуемая производительность
- ✅ Явная обработка ошибок

**Недостатки:**
- ❌ Легко проигнорировать ошибку
- ❌ Смешивание логики с обработкой ошибок
- ❌ Усложнение при множественных ошибках
- ❌ Проблемы с функциями, которые должны возвращать значение

#### Примеры реализации кодов возврата

**1. Коды возврата через препроцессор (макросы):**

```c
// errors.h - определения кодов ошибок через препроцессор
#define SUCCESS                    0
#define ERROR_FILE_NOT_FOUND      -1
#define ERROR_INSUFFICIENT_MEMORY -2
#define ERROR_INVALID_PARAMETER   -3
#define ERROR_ACCESS_DENIED       -4
#define ERROR_DISK_FULL          -5
#define ERROR_TIMEOUT            -6
#define ERROR_NETWORK_FAILURE    -7

// Макросы для проверки и распространения ошибок
#define CHECK_ERROR(expr) do { \
    int result = (expr); \
    if (result != SUCCESS) { \
        return result; \
    } \
} while(0)

#define RETURN_ON_ERROR(expr) do { \
    int err = (expr); \
    if (err != SUCCESS) { \
        cleanup(); \
        return err; \
    } \
} while(0)

// Использование в коде
int open_file(const char* filename, FILE** file) {
    if (!filename || !file) return ERROR_INVALID_PARAMETER;
    
    *file = fopen(filename, "r");
    if (!*file) {
        switch(errno) {
            case ENOENT: return ERROR_FILE_NOT_FOUND;
            case EACCES: return ERROR_ACCESS_DENIED;
            default: return ERROR_FILE_NOT_FOUND;
        }
    }
    
    return SUCCESS;
}

int process_file(const char* filename) {
    FILE* file;
    char* buffer = NULL;
    
    // Проверяем каждую операцию
    CHECK_ERROR(open_file(filename, &file));
    
    buffer = malloc(1024);
    if (!buffer) {
        fclose(file);
        return ERROR_INSUFFICIENT_MEMORY;
    }
    
    // Основная работа
    size_t bytes_read = fread(buffer, 1, 1024, file);
    if (bytes_read == 0 && ferror(file)) {
        free(buffer);
        fclose(file);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Очистка ресурсов
    free(buffer);
    fclose(file);
    return SUCCESS;
}

// Пример использования с обработкой ошибок
int main() {
    int result = process_file("data.txt");
    
    switch(result) {
        case SUCCESS:
            printf("File processed successfully\n");
            break;
        case ERROR_FILE_NOT_FOUND:
            printf("Error: File not found\n");
            break;
        case ERROR_INSUFFICIENT_MEMORY:
            printf("Error: Not enough memory\n");
            break;
        case ERROR_INVALID_PARAMETER:
            printf("Error: Invalid parameter\n");
            break;
        default:
            printf("Unknown error: %d\n", result);
    }
    
    return result == SUCCESS ? 0 : 1;
}
```

**2. Коды возврата через enum (перечисления):**

```c
// error_codes.h - типобезопасные коды ошибок
typedef enum {
    RESULT_SUCCESS = 0,
    RESULT_ERROR_FILE_NOT_FOUND,
    RESULT_ERROR_INSUFFICIENT_MEMORY, 
    RESULT_ERROR_INVALID_PARAMETER,
    RESULT_ERROR_ACCESS_DENIED,
    RESULT_ERROR_DISK_FULL,
    RESULT_ERROR_TIMEOUT,
    RESULT_ERROR_NETWORK_FAILURE,
    RESULT_ERROR_COUNT  // для валидации
} result_code_t;

// Структура для возвращения значения и ошибки
typedef struct {
    result_code_t code;
    union {
        void* ptr_value;
        int int_value;
        double double_value;
    } value;
} result_t;

// Макросы для создания результатов
#define MAKE_SUCCESS(val) ((result_t){RESULT_SUCCESS, .value.int_value = (val)})
#define MAKE_ERROR(code) ((result_t){(code), .value.int_value = 0})
#define IS_SUCCESS(result) ((result).code == RESULT_SUCCESS)
#define IS_ERROR(result) ((result).code != RESULT_SUCCESS)

// Функция для получения описания ошибки
const char* get_error_string(result_code_t code) {
    static const char* error_messages[] = {
        "Success",
        "File not found",
        "Insufficient memory",
        "Invalid parameter", 
        "Access denied",
        "Disk full",
        "Timeout",
        "Network failure"
    };
    
    if (code >= 0 && code < RESULT_ERROR_COUNT) {
        return error_messages[code];
    }
    return "Unknown error";
}

// Примеры использования enum-кодов
result_t divide_integers(int a, int b) {
    if (b == 0) {
        return MAKE_ERROR(RESULT_ERROR_INVALID_PARAMETER);
    }
    return MAKE_SUCCESS(a / b);
}

result_t open_and_read_file(const char* filename, char** buffer, size_t* size) {
    if (!filename || !buffer || !size) {
        return MAKE_ERROR(RESULT_ERROR_INVALID_PARAMETER);
    }
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        switch(errno) {
            case ENOENT: return MAKE_ERROR(RESULT_ERROR_FILE_NOT_FOUND);
            case EACCES: return MAKE_ERROR(RESULT_ERROR_ACCESS_DENIED);
            default: return MAKE_ERROR(RESULT_ERROR_FILE_NOT_FOUND);
        }
    }
    
    // Определяем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    *buffer = malloc(file_size + 1);
    if (!*buffer) {
        fclose(file);
        return MAKE_ERROR(RESULT_ERROR_INSUFFICIENT_MEMORY);
    }
    
    size_t bytes_read = fread(*buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        free(*buffer);
        *buffer = NULL;
        fclose(file);
        return MAKE_ERROR(RESULT_ERROR_FILE_NOT_FOUND);
    }
    
    (*buffer)[file_size] = '\0';
    *size = file_size;
    fclose(file);
    
    return MAKE_SUCCESS(0);
}

// Пример композиции функций с enum-кодами
result_t process_config_file(const char* config_path) {
    char* buffer = NULL;
    size_t size = 0;
    
    result_t read_result = open_and_read_file(config_path, &buffer, &size);
    if (IS_ERROR(read_result)) {
        return read_result;
    }
    
    // Парсинг конфигурации (упрощенный пример)
    if (strstr(buffer, "invalid_config") != NULL) {
        free(buffer);
        return MAKE_ERROR(RESULT_ERROR_INVALID_PARAMETER);
    }
    
    // Основная логика обработки
    printf("Config processed successfully, size: %zu bytes\n", size);
    
    free(buffer);
    return MAKE_SUCCESS(1);
}

// Использование в main с enum-кодами
int main() {
    result_t math_result = divide_integers(10, 2);
    if (IS_SUCCESS(math_result)) {
        printf("Division result: %d\n", math_result.value.int_value);
    } else {
        printf("Math error: %s\n", get_error_string(math_result.code));
    }
    
    result_t config_result = process_config_file("config.ini");
    if (IS_ERROR(config_result)) {
        printf("Config processing failed: %s\n", get_error_string(config_result.code));
        return 1;
    }
    
    return 0;
}
```

**Сравнение подходов:**

| Аспект | Макросы | Enum |
|--------|---------|------|
| Типобезопасность | ❌ Отсутствует | ✅ Есть |
| Читаемость кода | ⚠️ Средняя | ✅ Высокая |
| Возможность расширения | ⚠️ Требует осторожности | ✅ Простое добавление |
| Производительность | ✅ Максимальная | ✅ Высокая |
| Отладка | ❌ Сложная | ✅ Простая |
| Совместимость с C++ | ⚠️ Ограниченная | ✅ Хорошая |

#### Глобальные переменные (errno)

`errno` - это глобальная переменная (фактически макрос в современных реализациях), которая устанавливается библиотечными функциями для указания типа произошедшей ошибки.

**Преимущества errno:**
- ✅ **Стандартизованность**: Определен в стандарте C (ISO C90) и POSIX
- ✅ **Универсальность**: Используется множеством системных и библиотечных функций
- ✅ **Детальная информация**: Предоставляет специфические коды ошибок (ENOENT, EACCES, и т.д.)
- ✅ **Thread-safety**: В современных реализациях каждый поток имеет свой errno
- ✅ **Обратная совместимость**: Поддержка legacy кода
- ✅ **Низкие накладные расходы**: Простое присваивание значения
- ✅ **Интеграция с системными вызовами**: Прямая связь с операционной системой

```c
#include <errno.h>
#include <string.h>  // для strerror
#include <stdio.h>

// Демонстрация различных сценариев использования errno
void demonstrate_errno_usage() {
    // 1. Работа с файлами
    printf("=== File Operations ===\n");
    FILE* file = fopen("nonexistent_file.txt", "r");
    if (!file) {
        printf("fopen failed: errno=%d (%s)\n", errno, strerror(errno));
        // Обычно выведет: fopen failed: errno=2 (No such file or directory)
    }
    
    errno = 0; // Сброс перед следующей операцией
    
    // 2. Математические функции
    printf("\n=== Math Operations ===\n");
    double result = sqrt(-1.0);  // Недопустимая операция
    if (errno == EDOM) {
        printf("sqrt(-1): Domain error (errno=%d)\n", errno);
    }
    
    errno = 0;
    result = exp(1000.0);  // Переполнение
    if (errno == ERANGE) {
        printf("exp(1000): Range error - overflow (errno=%d)\n", errno);
    }
    
    // 3. Строковые операции
    printf("\n=== String Operations ===\n");
    errno = 0;
    long value = strtol("999999999999999999999", NULL, 10); // Переполнение
    if (errno == ERANGE) {
        printf("strtol overflow: errno=%d, value=%ld\n", errno, value);
    }
    
    // 4. Динамическая память  
    printf("\n=== Memory Operations ===\n");
    errno = 0;
    void* huge_block = malloc(SIZE_MAX); // Попытка выделить максимум памяти
    if (!huge_block && errno == ENOMEM) {
        printf("malloc failed: Not enough memory (errno=%d)\n", errno);
    }
}

// Пример надежной функции с использованием errno
int safe_file_copy(const char* source, const char* destination) {
    FILE* src = NULL, *dst = NULL;
    char buffer[4096];
    size_t bytes_read, bytes_written;
    int saved_errno;
    
    // Открываем исходный файл
    errno = 0;
    src = fopen(source, "rb");
    if (!src) {
        saved_errno = errno;
        printf("Cannot open source file '%s': %s\n", source, strerror(saved_errno));
        return -saved_errno;
    }
    
    // Открываем целевой файл
    errno = 0; 
    dst = fopen(destination, "wb");
    if (!dst) {
        saved_errno = errno;
        printf("Cannot create destination file '%s': %s\n", destination, strerror(saved_errno));
        fclose(src);
        return -saved_errno;
    }
    
    // Копируем содержимое
    while (1) {
        errno = 0;
        bytes_read = fread(buffer, 1, sizeof(buffer), src);
        
        if (bytes_read == 0) {
            if (feof(src)) {
                break; // Конец файла - нормальное завершение
            } else if (ferror(src)) {
                saved_errno = errno;
                printf("Error reading from '%s': %s\n", source, strerror(saved_errno));
                fclose(src);
                fclose(dst);
                remove(destination); // Удаляем частично скопированный файл
                return -saved_errno;
            }
        }
        
        bytes_written = fwrite(buffer, 1, bytes_read, dst);
        if (bytes_written != bytes_read) {
            saved_errno = errno;
            printf("Error writing to '%s': %s\n", destination, strerror(saved_errno));
            fclose(src);
            fclose(dst);
            remove(destination);
            return -saved_errno;
        }
    }
    
    fclose(src);
    if (fclose(dst) != 0) {
        saved_errno = errno;
        printf("Error closing '%s': %s\n", destination, strerror(saved_errno));
        return -saved_errno;
    }
    
    return 0; // Успех
}

// Расширенные коды ошибок POSIX
void demonstrate_posix_errno() {
    printf("\n=== Common POSIX errno values ===\n");
    printf("EPERM (1): %s\n", strerror(EPERM));       // Operation not permitted
    printf("ENOENT (2): %s\n", strerror(ENOENT));     // No such file or directory  
    printf("EINTR (4): %s\n", strerror(EINTR));       // Interrupted system call
    printf("EIO (5): %s\n", strerror(EIO));           // I/O error
    printf("ENXIO (6): %s\n", strerror(ENXIO));       // No such device or address
    printf("EACCES (13): %s\n", strerror(EACCES));    // Permission denied
    printf("EFAULT (14): %s\n", strerror(EFAULT));    // Bad address
    printf("EBUSY (16): %s\n", strerror(EBUSY));      // Device or resource busy
    printf("EEXIST (17): %s\n", strerror(EEXIST));    // File exists
    printf("ENOTDIR (20): %s\n", strerror(ENOTDIR));  // Not a directory
    printf("EISDIR (21): %s\n", strerror(EISDIR));    // Is a directory
    printf("EINVAL (22): %s\n", strerror(EINVAL));    // Invalid argument
    printf("EMFILE (24): %s\n", strerror(EMFILE));    // Too many open files
    printf("ENOSPC (28): %s\n", strerror(ENOSPC));    // No space left on device
    printf("EPIPE (32): %s\n", strerror(EPIPE));      // Broken pipe
    printf("EDOM (33): %s\n", strerror(EDOM));        // Math domain error
    printf("ERANGE (34): %s\n", strerror(ERANGE));    // Math range error
}
```

**Проблемы подхода с errno:**
- ❌ **Не thread-safe в старых реализациях** (решено в современных системах)
- ❌ **Может быть перезаписана последующими вызовами** - требует немедленной проверки
- ❌ **Требует проверки после каждого вызова функции**
- ❌ **Глобальное состояние** может приводить к неожиданному поведению
- ❌ **Не все функции устанавливают errno** корректно
- ❌ **Необходимость сброса errno** перед вызовом функций

**Современные улучшения errno:**

```c
// В современных системах errno - это макрос
#define errno (*__errno_location()) // Linux
// или
#define errno (*__error())          // macOS/BSD
// или  
#define errno (GetLastError())      // Windows (аналог)

// Это обеспечивает thread-safety
```

### Исключения (Exception)
Событие, которое возникает во время выполнения программы и нарушает нормальный поток выполнения инструкций.

**Основная идея:** Разделение основной логики программы и кода обработки ошибок.

```cpp
// C-подход: логика смешана с обработкой ошибок
int process_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return ERROR_FILE;          // ← обработка ошибки
    
    char* buffer = malloc(1024);
    if (!buffer) {                         // ← обработка ошибки
        fclose(file);                      // ← очистка
        return ERROR_MEMORY;               // ← обработка ошибки
    }
    
    size_t bytes = fread(buffer, 1, 1024, file);
    if (bytes == 0 && ferror(file)) {     // ← обработка ошибки
        free(buffer);                      // ← очистка
        fclose(file);                      // ← очистка
        return ERROR_READ;                 // ← обработка ошибки
    }
    
    // Основная логика занимает 20% кода!
    process_data(buffer, bytes);
    
    free(buffer);                          // ← очистка
    fclose(file);                          // ← очистка
    return SUCCESS;
}

// C++ подход: чистая логика
void process_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open file");
    
    std::vector<char> buffer(1024);
    file.read(buffer.data(), buffer.size());
    
    if (file.bad()) throw std::runtime_error("Read error");
    
    // Основная логика занимает 80% кода!
    process_data(buffer.data(), file.gcount());
    
    // Автоматическая очистка благодаря деструкторам!
}
```


**Ключевые характеристики:**
- Исключение **нельзя проигнорировать**
- Исключение **автоматически распространяется** по стеку вызовов
- **Автоматическая очистка ресурсов** при исключениях
- Исключение **прерывает текущий поток выполнения**

### Основные компоненты механизма исключений

#### throw - выбрасывание исключения

**Синтаксис:**
```cpp
throw expression;
```

**Примеры:**
```cpp
// Выбрасывание встроенных типов
throw 42;                           // int
throw "Error message";              // const char*
throw std::string("Error");         // std::string

// Выбрасывание объектов исключений
throw std::runtime_error("File not found");
throw std::invalid_argument("Invalid parameter");
throw std::bad_alloc();             // Исключение нехватки памяти

// Повторное выбрасывание текущего исключения
catch (...) {
    log_error("Caught unknown exception");
    throw;  // Пробрасываем дальше
}
```

#### try - блок защищенного кода

```cpp
try {
    // Код, который может выбросить исключение
    risky_operation();
    another_risky_operation();
    
    // Если исключение выброшено, выполнение прерывается
    // и управление передается в блок catch
    
} catch (const std::exception& e) {
    // Обработка исключения
    std::cerr << "Error: " << e.what() << std::endl;
}
```

####  catch - перехват исключений

**Множественные catch блоки:**
```cpp
try {
    risky_operation();
} 
catch (const std::bad_alloc& e) {
    // Специфическая обработка нехватки памяти
    std::cerr << "Out of memory: " << e.what() << std::endl;
    cleanup_memory();
}
catch (const std::runtime_error& e) {
    // Обработка runtime ошибок
    std::cerr << "Runtime error: " << e.what() << std::endl;
    log_error(e.what());
}
catch (const std::exception& e) {
    // Обработка всех остальных std::exception
    std::cerr << "General error: " << e.what() << std::endl;
}
catch (...) {
    // Перехват всех остальных исключений
    std::cerr << "Unknown error" << std::endl;
    throw; // Пробрасываем дальше
}
```

**Порядок catch блоков важен!** - от более специфичных к более общим.

#### Иерархия стандартных исключений

```
std::exception
├── std::bad_alloc
├── std::bad_cast
├── std::bad_typeid
└── std::logic_error
│   ├── std::invalid_argument
│   ├── std::domain_error
│   ├── std::length_error
│   └── std::out_of_range
└── std::runtime_error
    ├── std::range_error
    ├── std::overflow_error
    ├── std::underflow_error
    └── std::system_error
```

### Глубокое погружение в исключения: раскрутка стека (Stack Unwinding)

**Stack Unwinding** — это процесс автоматического вызова деструкторов для всех автоматических объектов, созданных с момента входа в блок `try` до точки возникновения исключения. Этот механизм является ключевым для обеспечения безопасности исключений в C++.

#### Как работает Stack Unwinding

```cpp
#include <iostream>
#include <stdexcept>

// Класс для демонстрации времени жизни объектов
class Demo {
    std::string name_;
public:
    Demo(const std::string& name) : name_(name) {
        std::cout << "Конструктор: " << name_ << std::endl;
    }
    
    ~Demo() {
        std::cout << "Деструктор: " << name_ << std::endl;
    }
    
    void do_work() {
        std::cout << "Работает: " << name_ << std::endl;
    }
};

void function_C() {
    Demo obj_c("Объект_C");
    obj_c.do_work();
    
    // Здесь возникает исключение
    throw std::runtime_error("Ошибка в функции C");
    
    // Эта строка никогда не выполнится
    std::cout << "После исключения в C" << std::endl;
}

void function_B() {
    Demo obj_b("Объект_B");  
    obj_b.do_work();
    
    {
        Demo obj_b_inner("Объект_B_внутренний");
        function_C(); // Вызов функции, которая выбросит исключение
    }
    
    // Эта строка не выполнится
    std::cout << "После вызова C в B" << std::endl;
}

void function_A() {
    Demo obj_a("Объект_A");
    obj_a.do_work();
    
    try {
        function_B();
    } catch (const std::exception& e) {
        std::cout << "Перехвачено исключение: " << e.what() << std::endl;
    }
    
    // Эта строка выполнится!
    std::cout << "Продолжение работы в A" << std::endl;
}

int main() {
    std::cout << "=== Демонстрация Stack Unwinding ===" << std::endl;
    function_A();
    std::cout << "=== Конец программы ===" << std::endl;
    return 0;
}

/*
Ожидаемый вывод:
=== Демонстрация Stack Unwinding ===
Конструктор: Объект_A
Работает: Объект_A
Конструктор: Объект_B
Работает: Объект_B
Конструктор: Объект_B_внутренний
Конструктор: Объект_C
Работает: Объект_C
Деструктор: Объект_C          // ← Stack unwinding начинается здесь
Деструктор: Объект_B_внутренний // ← Продолжается здесь
Деструктор: Объект_B           // ← И здесь
Перехвачено исключение: Ошибка в функции C
Продолжение работы в A
Деструктор: Объект_A          // ← Нормальное завершение
=== Конец программы ===
*/
```
#### Важные аспекты Stack Unwinding

**1. Гарантия вызова деструкторов:**
- Все полностью сконструированные автоматические объекты будут уничтожены
- Деструкторы вызываются в **обратном порядке** конструирования
- Частично сконструированные объекты не будут иметь вызов деструктора

**2. Исключения в деструкторах:**
```cpp
class ProblematicClass {
public:
    ~ProblematicClass() {
        // ОПАСНО! Исключение в деструкторе во время Stack Unwinding
        // приведет к вызову std::terminate
        if (unwinding_in_progress) {
            throw std::runtime_error("Ошибка в деструкторе"); // ← terminate!
        }
    }
};

class SafeClass {
public:
    ~SafeClass() noexcept {  // ← Всегда помечайте деструкторы как noexcept
        try {
            // Опасные операции в try-catch
            risky_cleanup();
        } catch (...) {
            // Логируем ошибку, но не пробрасываем исключение
            // std::cerr << "Ошибка при очистке ресурсов" << std::endl;
        }
    }
};
```

**3. Производительность Stack Unwinding:**
- Stack unwinding может быть дорогой операцией
- Требует поиска обработчиков исключений
- Вызов множественных деструкторов
- В критичных к производительности участках стоит минимизировать исключения

### Альтернативы исключениям в C++23 

#### std::expected<T, E> - подробное руководство

**`std::expected<T, E>`** - это шаблонный класс из C++23, который представляет результат операции, которая может завершиться успешно (возвращая значение типа `T`) или с ошибкой (возвращая ошибку типа `E`).

#### Основные возможности std::expected

```cpp
#include <expected>
#include <string>
#include <iostream>

// Простой пример использования
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}

// Более сложный пример с enum для ошибок
enum class MathError {
    DivisionByZero,
    Overflow,
    InvalidInput
};

std::expected<double, MathError> safe_sqrt(double value) {
    if (value < 0) {
        return std::unexpected(MathError::InvalidInput);
    }
    return std::sqrt(value);
}

// Демонстрация использования
void demonstrate_basic_expected() {
    // Успешный случай
    auto result1 = divide(10, 2);
    if (result1) {
        std::cout << "Результат: " << *result1 << std::endl;  // Результат: 5
    }
    
    // Случай с ошибкой
    auto result2 = divide(10, 0);
    if (!result2) {
        std::cout << "Ошибка: " << result2.error() << std::endl;  // Ошибка: Division by zero
    }
    
    // Использование value_or для значения по умолчанию
    auto result3 = divide(10, 0);
    int safe_value = result3.value_or(-1);
    std::cout << "Безопасное значение: " << safe_value << std::endl;  // Безопасное значение: -1
}
```

#### Монадический интерфейс std::expected

**Монадический подход** позволяет элегантно объединять операции, которые могут завершиться ошибкой, без необходимости явной проверки каждого результата.

```cpp
#include <expected>
#include <string>
#include <iostream>

// Цепочка операций с использованием монадических функций
std::expected<int, std::string> parse_int(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (...) {
        return std::unexpected("Invalid number format");
    }
}

std::expected<int, std::string> multiply_by_2(int value) {
    if (value > 1000000) {
        return std::unexpected("Value too large for multiplication");
    }
    return value * 2;
}

std::expected<std::string, std::string> to_string(int value) {
    return std::to_string(value);
}

// Демонстрация монадических операций
void demonstrate_monadic_operations() {
    std::cout << "\n=== Монадические операции ===" << std::endl;
    
    // 1. and_then - продолжает только если нет ошибки
    auto result1 = parse_int("42")
        .and_then(multiply_by_2)
        .and_then([](int val) -> std::expected<std::string, std::string> {
            return "Result: " + std::to_string(val);
        });
    
    if (result1) {
        std::cout << "Успех: " << *result1 << std::endl;  // Успех: Result: 84
    }
    
    // 2. or_else - выполняется только при ошибке
    auto result2 = parse_int("invalid")
        .or_else([](const std::string& error) -> std::expected<int, std::string> {
            std::cout << "Обработка ошибки: " << error << std::endl;
            return 0; // Значение по умолчанию
        })
        .and_then(multiply_by_2);
    
    if (result2) {
        std::cout << "Восстановление после ошибки: " << *result2 << std::endl;
    }
    
    // 3. transform - преобразует значение при успехе
    auto result3 = parse_int("123")
        .transform([](int val) { return val * 3; })  // int -> int
        .transform([](int val) { return std::to_string(val); }); // int -> string
    
    if (result3) {
        std::cout << "Трансформация: " << *result3 << std::endl;  // Трансформация: 369
    }
    
    // 4. transform_error - преобразует ошибку при неудаче
    auto result4 = parse_int("bad_input")
        .transform_error([](const std::string& error) {
            return "Улучшенное сообщение об ошибке: " + error;
        });
    
    if (!result4) {
        std::cout << result4.error() << std::endl;
    }
}

// Сложный пример с файловыми операциями
enum class FileError {
    NotFound,
    AccessDenied, 
    ReadError,
    WriteError
};

class FileProcessor {
public:
    std::expected<std::string, FileError> read_file(const std::string& filename) {
        // Имитация чтения файла
        if (filename == "missing.txt") {
            return std::unexpected(FileError::NotFound);
        }
        if (filename == "protected.txt") {
            return std::unexpected(FileError::AccessDenied);
        }
        return "file content for " + filename;
    }
    
    std::expected<std::string, FileError> process_content(const std::string& content) {
        if (content.empty()) {
            return std::unexpected(FileError::ReadError);
        }
        return "processed: " + content;
    }
    
    std::expected<void, FileError> write_result(const std::string& result) {
        if (result.length() > 100) {
            return std::unexpected(FileError::WriteError);
        }
        std::cout << "Записан результат: " << result << std::endl;
        return {};
    }
};

void demonstrate_file_processing() {
    std::cout << "\n=== Обработка файлов с std::expected ===" << std::endl;
    
    FileProcessor processor;
    
    // Успешная цепочка операций
    auto success_result = processor.read_file("data.txt")
        .and_then([&](const std::string& content) {
            return processor.process_content(content);
        })
        .and_then([&](const std::string& processed) -> std::expected<void, FileError> {
            return processor.write_result(processed);
        });
    
    if (!success_result) {
        std::cout << "Ошибка обработки файла" << std::endl;
    }
    
    // Обработка ошибки с восстановлением
    auto recovery_result = processor.read_file("missing.txt")
        .or_else([](FileError error) -> std::expected<std::string, FileError> {
            if (error == FileError::NotFound) {
                std::cout << "Файл не найден, используем значение по умолчанию" << std::endl;
                return "default content";
            }
            return std::unexpected(error);
        })
        .and_then([&](const std::string& content) {
            return processor.process_content(content);
        });
    
    if (recovery_result) {
        std::cout << "Результат с восстановлением: " << *recovery_result << std::endl;
    }
}
```

#### Монадический подход - суть и преимущества

**Монада** - это абстрактная структура данных, которая позволяет составлять функции, работающие с контекстом (в случае `std::expected` - контекст успеха/ошибки).

**Три основных принципа монад:**
1. **Функтор (Functor)** - можно применять функции к значениям внутри контекста
2. **Аппликативный функтор** - можно применять функции внутри контекста к значениям внутри контекста  
3. **Монада** - можно составлять функции, которые возвращают значения в контексте

**Преимущества монадического подхода:**
- ✅ **Композируемость** - легко объединять операции
- ✅ **Читаемость** - код выглядит как pipeline обработки
- ✅ **Безопасность** - автоматическая обработка ошибок
- ✅ **Отсутствие вложенности** - no deep if-else nesting
- ✅ **Ленивые вычисления** - операции не выполняются при ошибке

```cpp
// Традиционный подход - много проверок
std::optional<std::string> traditional_approach(const std::string& input) {
    auto parsed = parse_int(input);
    if (!parsed) return std::nullopt;
    
    auto doubled = multiply_by_2(*parsed);
    if (!doubled) return std::nullopt;
    
    auto stringified = to_string(*doubled);
    if (!stringified) return std::nullopt;
    
    return *stringified;
}

// Монадический подход - чистый pipeline
auto monadic_approach(const std::string& input) {
    return parse_int(input)
        .and_then(multiply_by_2)
        .and_then(to_string);
}
```

#### Рекомендации: когда использовать std::expected vs exceptions

**Используйте std::expected когда:**

| Сценарий | Причина | Пример |
|----------|---------|---------|
| **Ожидаемые ошибки** | Ошибки являются частью нормального потока выполнения | Парсинг пользовательского ввода, валидация данных |
| **Высокая частота ошибок** | Ошибки происходят часто | Сетевые запросы, файловые операции |
| **Производительность критична** | Избегание overhead от stack unwinding | Игровые движки, HFT, embedded системы |
| **Явная обработка ошибок** | Вынуждение проверки результата на месте вызова | API функции, библиотечный код |
| **Функциональный стиль** | Использование монадических операций | Data processing pipelines |
| **Детальная информация об ошибке** | Нужны специфичные коды ошибок | Различные типы валидации |

```cpp
// Идеально для std::expected
enum class ValidationError {
    EmptyInput,
    TooShort,
    TooLong,
    InvalidCharacters
};

std::expected<std::string, ValidationError> validate_username(const std::string& input) {
    if (input.empty()) return std::unexpected(ValidationError::EmptyInput);
    if (input.length() < 3) return std::unexpected(ValidationError::TooShort);
    if (input.length() > 20) return std::unexpected(ValidationError::TooLong);
    
    // Проверка на недопустимые символы
    for (char c : input) {
        if (!std::isalnum(c) && c != '_') {
            return std::unexpected(ValidationError::InvalidCharacters);
        }
    }
    
    return input;
}
```

**Используйте exceptions когда:**

| Сценарий | Причина | Пример |
|----------|---------|---------|
| **Исключительные ситуации** | Ошибки редки и неожиданны | Нехватка памяти, системные сбои |
| **Невозможность продолжения** | Ошибка делает дальнейшую работу невозможной | Критические системные ошибки |
| **Глубокая вложенность** | Ошибка должна всплыть через много уровней | Парсеры, recursive algorithms |
| **Совместимость с legacy кодом** | Существующий код использует исключения | Интеграция со старыми системами |
| **RAII критичен** | Необходима гарантированная очистка ресурсов | Работа с файлами, сетью, GPU |
| **Быстрый путь выполнения** | Оптимизация для случая без ошибок | Hot paths в производительном коде |

```cpp
// Хорошо подходит для exceptions
class DatabaseConnection {
public:
    void connect(const std::string& connection_string) {
        // Подключение к БД - редкая операция, ошибка критична
        if (!attempt_connection(connection_string)) {
            throw std::runtime_error("Failed to connect to database");
        }
    }
    
    void execute_query(const std::string& query) {
        // Если нет соединения - это исключительная ситуация
        if (!is_connected()) {
            throw std::logic_error("Not connected to database");
        }
        
        // Выполнение запроса
    }
};
```

**Гибридный подход:**
```cpp
class APIClient {
public:
    // Для ожидаемых ошибок - expected
    std::expected<UserData, APIError> get_user(int user_id) {
        if (user_id <= 0) {
            return std::unexpected(APIError::InvalidUserId);
        }
        
        try {
            // Неожиданные ошибки остаются исключениями
            auto response = http_client_.get("/users/" + std::to_string(user_id));
            return parse_user_data(response);
        } catch (const NetworkException& e) {
            // Конвертируем в expected если нужно
            return std::unexpected(APIError::NetworkError);
        }
    }
};
```

**Резюме рекомендаций:**

- **`std::expected`** - для предсказуемых ошибок и явного error handling
- **`exceptions`** - для исключительных ситуаций и когда нужен RAII
- **Комбинированный подход** часто наиболее практичен
- **Последовательность** в проекте важнее чем идеальный выбор инструмента

## 📋 **1.2: RAII и гарантии безопасности относительно исключений (Exception Safety)**

### Определение и философия RAII (Resource Acquisition is Initialization)

**RAII** - это идиома программирования, которая связывает время жизни ресурса со временем жизни объекта.

**Основные принципы:**
1. **Ресурс приобретается в конструкторе** объекта
2. **Ресурс освобождается в деструкторе** объекта  
3. **Время жизни ресурса = время жизни объекта**
4. **Автоматическое управление** - программист не думает об освобождении

#### Проблема без RAII

```cpp
void problematic_function() {
    // Приобретение ресурсов
    FILE* file = fopen("data.txt", "r");
    char* buffer = new char[1024];
    HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
    
    // Работа с ресурсами
    if (some_condition) {
        // ПРОБЛЕМА: забыли освободить ресурсы!
        return;
    }
    
    if (another_condition) {
        // ПРОБЛЕМА: частичное освобождение!
        delete[] buffer;
        return;
    }
    
    // Обычное освобождение (может не выполниться при исключении)
    CloseHandle(mutex);
    delete[] buffer;
    fclose(file);
}
```

**Проблемы:**
- ❌ Множественные точки выхода требуют дублирования кода очистки
- ❌ Легко забыть освободить ресурс
- ❌ Исключения могут прервать освобождение ресурсов
- ❌ Сложность поддержки при изменении кода

#### Решение с RAII

```cpp
// RAII wrapper для FILE*
class FileWrapper {
    FILE* file_;
    std::string filename_;
    
public:
    FileWrapper(const std::string& filename, const char* mode) 
        : filename_(filename) {
        file_ = fopen(filename_.c_str(), mode);
        if (!file_) {
            throw std::runtime_error("Cannot open file: " + filename_);
        }
        std::cout << "File opened: " << filename_ << std::endl;
    }
    
    ~FileWrapper() {
        if (file_) {
            fclose(file_);
            std::cout << "File closed: " << filename_ << std::endl;
        }
    }
    
    // Запрещаем копирование
    FileWrapper(const FileWrapper&) = delete;
    FileWrapper& operator=(const FileWrapper&) = delete;
    
    // Разрешаем перемещение
    FileWrapper(FileWrapper&& other) noexcept 
        : file_(other.file_), filename_(std::move(other.filename_)) {
        other.file_ = nullptr;
    }
    
    FILE* get() const { return file_; }
    operator FILE*() const { return file_; }
};

// RAII wrapper для динамической памяти  
template<typename T>
class ArrayWrapper {
    T* data_;
    size_t size_;
    
public:
    explicit ArrayWrapper(size_t size) : size_(size) {
        data_ = new T[size_];
        std::cout << "Array allocated: " << size_ << " elements" << std::endl;
    }
    
    ~ArrayWrapper() {
        delete[] data_;
        std::cout << "Array deallocated" << std::endl;
    }
    
    T* get() const { return data_; }
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    size_t size() const { return size_; }
};

// Использование RAII
void raii_function() {
    // Ресурсы приобретаются при создании объектов
    FileWrapper file("data.txt", "r");      // Автоматическое открытие
    ArrayWrapper<char> buffer(1024);        // Автоматическое выделение памяти
    
    // Работаем с ресурсами
    if (some_condition) {
        return;  // ✅ Автоматическая очистка!
    }
    
    if (another_condition) {
        throw std::runtime_error("Error");  // ✅ Автоматическая очистка!
    }
    
    // Даже при нормальном завершении - автоматическая очистка!
    // Деструкторы вызываются автоматически при выходе из области видимости
}
```

#### Умные указатели как реализация RAII:
```cpp
auto resource = std::make_unique<Resource>();
// Автоматическое освобождение при любом выходе из области видимости
```

### Уровни гарантий Exception Safety

Exception Safety - это гарантии о состоянии программы при возникновении исключений.
#### No Exception Safety (Нет гарантий)

При исключении могут произойти утечки ресурсов и объект может остаться в некорректном состоянии.

```cpp
class UnsafeClass {
    int* data1_;
    int* data2_;
    size_t size_;
    
public:
    void unsafe_operation(size_t new_size) {
        // ОПАСНО: если второй new выбросит исключение,
        // первый блок памяти утечет
        data1_ = new int[new_size];  // Может выбросить bad_alloc
        data2_ = new int[new_size];  // Может выбросить bad_alloc
        
        size_ = new_size;
        
        // Если исключение произошло, объект в некорректном состоянии!
    }
};
```

#### Basic Guarantee (Базовые гарантии)

При исключении не происходит утечек ресурсов, и все объекты остаются в валидном состоянии, но состояние может быть изменено.

```cpp
class BasicSafeClass {
    std::vector<int> data_;
    size_t processed_count_;
    
public:
    BasicSafeClass() : processed_count_(0) {}
    
    void process_items(const std::vector<int>& items) {
        for (const int& item : items) {
            data_.push_back(process(item));  // Может выбросить исключение
            ++processed_count_;              // Инкремент может не произойти
        }
        
        // При исключении: processed_count_ может не соответствовать data_.size()
        // НО: нет утечек памяти, объект в валидном состоянии
    }
    
    // Инварианты объекта
    bool invariant_check() const {
        return processed_count_ <= data_.size();  // Всегда должно быть true
    }
    
private:
    int process(int value) {
        if (value < 0) throw std::invalid_argument("Negative value");
        return value * 2;
    }
};
```

#### Strong Guarantee (Строгие гарантии)

При исключении состояние объекта остается точно таким же, как до начала операции (commit or rollback semantics).

```cpp
class StrongSafeClass {
    std::vector<int> data_;
    
public:
    void push_back_strong(const int& value) {
        // Создаем копию для rollback
        std::vector<int> temp_data = data_;
        
        try {
            temp_data.push_back(value);  // Может выбросить исключение
            data_ = std::move(temp_data); // Этот шаг noexcept для move
        } catch (...) {
            // data_ остается неизменным
            throw;
        }
        
        // Либо полный успех, либо data_ не изменилось
    }
    
    void reserve_strong(size_t new_capacity) {
        // Используем тот факт, что std::vector::reserve обеспечивает strong guarantee
        data_.reserve(new_capacity);
        
        // Если reserve выбрасывает исключение, data_ остается неизменным
    }
};
```

#### No-Throw Guarantee (Гарантия отсутствия исключений)

 Операция гарантированно не выбросит исключения. Спецификатор `noexcept `

```cpp
class NoThrowClass {
    std::atomic<int> counter_;
    
public:
    NoThrowClass() noexcept : counter_(0) {}
    
    // Гарантированно безопасные операции
    void increment() noexcept {
        counter_.fetch_add(1);  // Атомарные операции не бросают исключения
    }
    
    int get_count() const noexcept {
        return counter_.load();
    }
    
    void reset() noexcept {
        counter_.store(0);
    }
    
    // Деструктор всегда должен быть noexcept
    ~NoThrowClass() noexcept = default;
    
    // Swap операции обычно noexcept
    void swap(NoThrowClass& other) noexcept {
        int temp = counter_.exchange(other.counter_.load());
        other.counter_.store(temp);
    }
};
```

### Практическое применение концепции безопасного периметра

Безопасный периметр в C++ основывается на создании концентрических зон безопасности, где каждый уровень предоставляет определенные гарантии при возникновении исключений.
#### Архитектурные паттерны

При проектировании системы с использованием концепции безопасного периметра рекомендуется следующая архитектура:

1. **Ядро системы** - код с строгими гарантиями безопасности, использующий RAII и умные указатели
2. **Промежуточные слои** - компоненты с базовыми гарантиями безопасности
3. **Границы взаимодействия** - слои трансляции исключений для взаимодействия с внешними системами
4. **Внешние интерфейсы** - C-совместимые интерфейсы без исключений

#### Рекомендации по реализации

**Используйте RAII везде, где возможно**. Каждый ресурс должен управляться объектом RAII, обеспечивающим автоматическое освобождение.

**Минимизируйте области с отсутствием гарантий безопасности**. Такой код должен быть изолирован и тщательно протестирован.

**Создавайте четкие границы исключений**. Все исключения должны обрабатываться на соответствующих уровнях архитектуры.

**Предпочитайте композицию наследованию**. Это упрощает управление ресурсами и обеспечение гарантий безопасности.

### Copy-and-Swap идиома

Паттерн, который обеспечивает strong exception safety для операторов присваивания.

```cpp
template<typename T>
class SafeContainer {
    T* data_;
    size_t size_;
    size_t capacity_;
    
    // Безопасная функция копирования
    T* safe_copy(const T* source, size_t count) {
        if (count == 0) return nullptr;
        
        T* dest = new T[count];  // Может выбросить bad_alloc
        
        try {
            for (size_t i = 0; i < count; ++i) {
                dest[i] = source[i];  // Может выбросить исключение при копировании
            }
        } catch (...) {
            delete[] dest;  // Освобождаем частично заполненный массив
            throw;
        }
        
        return dest;
    }
    
public:
    SafeContainer() : data_(nullptr), size_(0), capacity_(0) {}
    
    SafeContainer(size_t capacity) : capacity_(capacity), size_(0) {
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
            SafeContainer temp(other);      // Создаем копию (может выбросить)
            swap(temp);                     // Обмениваем (noexcept)
        }
        return *this;
        // temp автоматически уничтожается, освобождая старые данные
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
    
    // noexcept swap - ключевая функция для copy-and-swap
    void swap(SafeContainer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

// Глобальная swap функция
template<typename T>
void swap(SafeContainer<T>& a, SafeContainer<T>& b) noexcept {
    a.swap(b);
}
```




---
## 📋 **Практические задания**
[[tasks-part-1]]


---

## 📚 Материалы и ресурсы

1. **Bjarne Stroustrup** - "Exception Safety: Concepts and Techniques"
2. **Herb Sutter** - "Exceptional C++: 47 Engineering Puzzles"
3. **Scott Meyers** - "Effective Modern C++" (Items 14-17)
4. **Microsoft C++ Team** - "Modern C++ best practices for exceptions"
5. **cppreference.com** - исключения и RAII
