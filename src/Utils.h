#ifndef GIT_UTILS_H
#define GIT_UTILS_H

#include <string>

class Utils {
public:
    static std::string calculateSHA1(const std::string& input);
    static uint32_t leftRotate(uint32_t value, uint32_t shift);
};

#endif
