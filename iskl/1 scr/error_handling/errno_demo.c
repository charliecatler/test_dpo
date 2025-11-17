#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

// Демонстрация различных сценариев использования errno
void demonstrate_errno_usage() {

    printf("\n*** errno Usage Examples ***\n");
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
    //printf("\n=== Memory Operations ===\n");
    //errno = 0;
    //void* huge_block = malloc(SIZE_MAX); // Попытка выделить максимум памяти
    //if (!huge_block && errno == ENOMEM) {
    //    printf("malloc failed: Not enough memory (errno=%d)\n", errno);
    //}
}

// Расширенные коды ошибок POSIX
void demonstrate_posix_errno() {
    printf("=== Common POSIX errno values ===\n");
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

int main() {
   
    demonstrate_posix_errno();
    demonstrate_errno_usage();
    
    return 0;
}
