// error_codes.hpp
#pragma once
#include <string>
#include <variant>
#include <vector>
#include <fstream>
#include <stdexcept>

enum class ResultCode {
    Success = 0,
    FileNotFound,
    InsufficientMemory,
    InvalidParameter,
    AccessDenied,
    DiskFull,
    Timeout,
    NetworkFailure,
    Count
};

struct Result {
    ResultCode code;
    std::variant<int, double, void*> value;
};

inline Result MakeSuccess(int val)      { return {ResultCode::Success, val}; }
inline Result MakeError(ResultCode code){ return {code, 0}; }
inline bool IsSuccess(const Result& r)  { return r.code == ResultCode::Success; }
inline bool IsError(const Result& r)    { return r.code != ResultCode::Success; }

inline const char* GetErrorString(ResultCode code) {
    static const char* err[] = {
        "Success",
        "File not found",
        "Insufficient memory",
        "Invalid parameter",
        "Access denied",
        "Disk full",
        "Timeout",
        "Network failure"
    };
    int idx = static_cast<int>(code);
    return (idx >= 0 && idx < static_cast<int>(ResultCode::Count)) ? err[idx] : "Unknown error";
}
