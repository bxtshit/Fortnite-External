#pragma once

#include <cstdio>  

template <typename... Args>
void print(const char* format, Args... args)
{
    std::printf(format, args...);
}
