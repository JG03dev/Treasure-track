#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>

// ANSI escape codes for text color
#define GREEN_TEXT "\033[32m"
#define RED_TEXT "\033[31m"
#define RESET_COLOR "\033[0m"

inline void printPassMessage(const char* message)
{
    std::cout << GREEN_TEXT << "PASS: " << message << RESET_COLOR << std::endl;
};
inline void printFailMessage(const char* message)
{
    std::cerr << RED_TEXT << "FAIL: " << message << RESET_COLOR << std::endl;
};

inline int Assert_bool(bool cond, const char* output)
{
    if (cond)
    {
        printPassMessage(output);
        return 0;
    }
    else
    {
        printFailMessage(output);
        return 1;
    }
}
#endif // TEST_UTILS_H
