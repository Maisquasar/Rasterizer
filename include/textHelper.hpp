#pragma once

#include <stdint.h>

namespace text
{
    // Function for parsing .obj models.
    inline bool compareWord(const char *buff, const int64_t index, const int64_t maxSize, const char *inputWord);
    inline int countSlash(const char *buff, int64_t index, const int64_t maxSize);
    inline int64_t skipCharSafe(const char *buff, int64_t index, const int64_t maxSize);
    inline int64_t skipChar(const char *buff, int64_t index, const int64_t maxSize, const char cr);
    inline int64_t endLine(const char *buff, int64_t index, const int64_t maxSize);
    inline int64_t getInt(const char* buf, int64_t index, const int64_t maxSize);
    inline double getFloat(const char* buf, int64_t index, const int64_t maxSize);
}

#include <textHelper.inl>