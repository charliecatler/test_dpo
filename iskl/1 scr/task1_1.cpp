// error_codes.cpp
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "task1_1.hpp"

Result DivideIntegers(int a, int b)
{
    if (b == 0)
    {
        // return MakeError(ResultCode::InvalidParameter);
        // TODO: заменить на исключение std::invalid_argument
        throw std::invalid_argument("b = 0\n");
    }
    return MakeSuccess(a / b);
}

Result OpenAndReadFile(const std::string &filename, std::vector<char> &buffer)
{

    if (filename.empty())
    {
        // return MakeError(ResultCode::InvalidParameter);
        // TODO: заменить на исключение std::invalid_argument
        throw std::invalid_argument("filename is empty\n");
    }

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        // return MakeError(ResultCode::FileNotFound);
        // TODO: заменить на исключение std::runtime_error
        throw std::runtime_error("file is not opened\n");
    }

    std::streamsize size = file.tellg();
    if (size < 0)
    {
        // return MakeError(ResultCode::FileNotFound);
        throw std::invalid_argument("size = 0\n");
    }

    file.seekg(0, std::ios::beg);
    buffer.resize(static_cast<size_t>(size));

    if (!file.read(buffer.data(), size))
    {
        // return MakeError(ResultCode::FileNotFound);
        // TODO: заменить на исключение std::runtime_error
        throw std::runtime_error("file is not read\n");
    }

    return MakeSuccess(0);
}

Result ProcessConfigFile(const std::string &configPath)
{
    std::vector<char> buffer;
    Result readRes = OpenAndReadFile(configPath, buffer);

    // if (IsError(readRes))
    // {
    //     return readRes; // TODO: обработать здесь или перенести в main ?
    // }

    // some mock processing here

    std::cout << "Config processed successfully, size " << buffer.size() << " bytes\n";
    return MakeSuccess(1);
}

int main()
{
    // TODO: Обернуть вызовы функций в try-catch и вывести инфо о исключениях
    // Что будет, если исключения не будут пойманы?

    try
    {
        Result resDiv = DivideIntegers(5, 0);
        // std::cout << GetErrorString(resDiv.code) << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        Result resCfg = ProcessConfigFile("config.dat");
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Error readRes\n");
    }

    return 0;
}
