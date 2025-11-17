#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#include <stdlib.h>
#include <string.h>

/**
 * @brief Перечисление кодов ошибок
 * 
 * Предоставляет типобезопасный способ возврата информации об ошибке
 * вместо использования магических чисел или errno.
 */
typedef enum {
    RESULT_SUCCESS = 0,                     ///< Успешное выполнение
    RESULT_ERROR_FILE_NOT_FOUND,            ///< Файл не найден
    RESULT_ERROR_INSUFFICIENT_MEMORY,       ///< Недостаточно памяти
    RESULT_ERROR_INVALID_PARAMETER,         ///< Неверный параметр
    RESULT_ERROR_ACCESS_DENIED,             ///< Доступ запрещен
    RESULT_ERROR_DISK_FULL,                 ///< Диск переполнен
    RESULT_ERROR_TIMEOUT,                   ///< Истек таймаут
    RESULT_ERROR_NETWORK_FAILURE,           ///< Сетевая ошибка
    RESULT_ERROR_COUNT                      ///< Количество кодов ошибок
} result_code_t;

/**
 * @brief Структура для возврата результата с информацией об ошибке
 * 
 * Позволяет одновременно возвращать значение и статус выполнения функции.
 * Используется с помощью макросов IS_SUCCESS() и IS_ERROR().
 */
typedef struct {
    result_code_t code;                     ///< Код результата/ошибки
    union {
        void* ptr_value;                    ///< Значение указателя
        int int_value;                      ///< Целое число
        double double_value;                ///< Число с плавающей точкой
    } value;                                ///< Дополнительное значение
} result_t;

/**
 * @brief Макрос для создания результата успеха с значением
 * @param val Возвращаемое значение
 * @return result_t структура с кодом успеха и значением
 */
#define MAKE_SUCCESS(val) ((result_t){RESULT_SUCCESS, .value.int_value = (val)})

/**
 * @brief Макрос для создания результата ошибки
 * @param code Код ошибки (result_code_t)
 * @return result_t структура с кодом ошибки
 */
#define MAKE_ERROR(code) ((result_t){(code), .value.int_value = 0})

/**
 * @brief Проверка успешного результата
 * @param result Структура результата
 * @return true если результат успешен, false иначе
 */
#define IS_SUCCESS(result) ((result).code == RESULT_SUCCESS)

/**
 * @brief Проверка результата на наличие ошибки
 * @param result Структура результата
 * @return true если произошла ошибка, false иначе
 */
#define IS_ERROR(result) ((result).code != RESULT_SUCCESS)

/**
 * @brief Получить строковое описание кода ошибки
 * 
 * Преобразует числовой код ошибки в понятное для человека описание.
 * 
 * @param code Код ошибки
 * @return Строка с описанием ошибки (не NULL)
 */
[[nodiscard]] 
const char* get_error_string(result_code_t code);

/**
 * @brief Функция деления двух чисел с проверкой на ноль
 * 
 * Демонстрирует использование result_t для возврата ошибок
 * вместо прямого возврата результата.
 * 
 * @param a Делимое
 * @param b Делитель
 * @return result_t с результатом деления (a / b) или ошибкой
 */
[[nodiscard]] 
result_t divide_integers(int a, int b);

/**
 * @brief Открыть и прочитать файл
 * 
 * Безопасное открытие файла с полной проверкой ошибок.
 * Память для буфера выделяется внутри функции.
 * 
 * @param filename Имя файла для открытия
 * @param buffer   Указатель на буфер для результата (выделяется функцией)
 * @param size     Указатель на переменную для размера прочитанных данных
 * 
 * @return result_t с кодом успеха или ошибки
 * 
 * @note Вызывающий код должен освободить буфер через free()
 * @note При ошибке буфер остается неинициализированным
 */
[[nodiscard]] 
result_t open_and_read_file(const char* filename, char** buffer, size_t* size);

/**
 * @brief Обработать конфигурационный файл
 * 
 * Открывает, читает и валидирует конфигурационный файл.
 * Демонстрирует цепочку вызовов с обработкой ошибок.
 * 
 * @param config_path Путь к конфигурационному файлу
 * 
 * @return result_t с кодом успеха или ошибки
 */
[[nodiscard]] 
result_t process_config_file(const char* config_path);

#endif // ERROR_CODES_H
