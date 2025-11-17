cmake_minimum_required(VERSION 3.19.0)

set (PROJECT_NAME gdb_workshop)
project(${PROJECT_NAME})

# настраиваем стандарт С++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка режима сборки
#set(CMAKE_BUILD_TYPE Debug)

# устанавливаем флаги сборки
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# настраиваем каталоги сборки
set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# раздельные каталоги для debug и release сборок
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/debug)


## part 2
add_executable(std_thread_demo src/multithreading/std_thread_demo.cpp)
add_executable(std_jthread_demo src/multithreading/std_jthread_demo.cpp)
add_executable(task2_1 src/task2_1.cpp)
add_executable(task2_2 src/task2_2.cpp)
add_executable(mutex_demo src/multithreading/std_mutex_example.cpp)
add_executable(cond_vars  src/multithreading/std_cond_var.cpp)
add_executable(latch_demo src/multithreading/std_latch_demo.cpp)
add_executable(barrier_demo src/multithreading/std_barrier_demo.cpp)
add_executable(async_demo src/multithreading/std_async_future_sum.cpp)
