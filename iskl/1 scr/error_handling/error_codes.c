#include "error_codes.h"
#include <stdio.h>
#include <errno.h>

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
    errno = 0;
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
    if (bytes_read != (size_t)file_size) {
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

int main(){

    result_t res_div = divide_integers(5,0);
    const char* err_msg = get_error_string(res_div.code);
    printf("%s\n", err_msg);

    result_t res_cfg =  process_config_file("config.dat");
    err_msg = get_error_string(res_cfg.code);
    printf("%s\n", err_msg);

    return 0;
}