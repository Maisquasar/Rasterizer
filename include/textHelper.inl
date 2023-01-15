#include "textHelper.hpp"

inline bool text::compareWord(const char *buff, const int64_t index, const int64_t maxSize, const char *inputWord)
{
    for (int64_t i = 0; inputWord[i] != 0; i++)
    {
        if ((index + i > maxSize) || (inputWord[i] != buff[index + i]))
            return false;
    }
    return true;
}

inline int text::countSlash(const char *buff, int64_t index, const int64_t maxSize)
{
    int result = 0;
    while (index < maxSize && buff[index] != ' ' && buff[index] != '\n')
    {
        if (buff[index] == '/')
        {
            if (buff[index + 1] == '/')
                return -1;
            else
                result += 1;
        }
        index++;
    }
    return result;
}

inline int64_t text::skipCharSafe(const char *buff, int64_t index, const int64_t maxSize)
{
    while (index < maxSize && buff[index] != ' ' && buff[index] != '\n' && buff[index] != '/')
    {
        index += 1;
    }
    if (buff[index] == ' ')
    {
        while (index < maxSize && buff[index] == ' ')
        {
            index += 1;
        }
        return index;
    }
    return index + 1;
}

inline int64_t text::skipChar(const char *buff, int64_t index, const int64_t maxSize, const char cr)
{
    while (index < maxSize && buff[index] != cr)
        index += 1;
    return index + 1;
}

inline int64_t text::endLine(const char *buff, int64_t index, const int64_t maxSize)
{
    return skipChar(buff, index, maxSize, '\n');
}

int64_t text::getInt(const char* buf, int64_t index, const int64_t maxSize)
{
    int64_t value = 0;
    if (index >= maxSize) return value;
    char n = buf[index];
    bool negative = false;
    if (n == '-')
    {
        negative = true;
        index++;
        n = buf[index];
    }
    while (n >= '0' && n <= '9' && index < maxSize)
    {
        value *= 10;
        value += n - '0';
        index++;
        n = buf[index];
    }
    return (negative ? -value : value);
}

double text::getFloat(const char* buf, int64_t index, const int64_t maxSize)
{
    double value = 0;
    if (index >= maxSize) return value;
    char n = buf[index];
    bool negative = false;
    if (n == '-')
    {
        negative = true;
        index++;
        n = buf[index];
    }
    while (n >= '0' && n <= '9' && index < maxSize)
    {
        value *= 10;
        value += n - '0';
        index++;
        n = buf[index];
    }
    if (index >= maxSize) return (negative ? -value : value);
    if (n != '.') return (negative ? -value : value);
    index++;
    n = buf[index];
    size_t decimal = 10;
    while (n >= '0' && n <= '9' && decimal < 1000000000000000llu && index < maxSize)
    {
        value = value + (double)(n - '0') / decimal;
        decimal *= 10;
        index++;
        n = buf[index];
    }
    return (negative ? -value : value);
}