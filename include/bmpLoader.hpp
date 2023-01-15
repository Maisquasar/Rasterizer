#pragma once

namespace utils
{
    //LoadBMP function
    unsigned char* loadBMP(const char* filename, int* widthOut, int* heightOut, int* pixelSize);
}