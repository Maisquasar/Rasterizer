#include <iostream>
#include <fstream>
#include <cstring>

#include <bmpLoader.hpp>

#pragma pack(push, 1)
struct BMPHeader
{
    char signature[2];
    unsigned int fileSize;
    char reserved1[2];
    char reserved2[2];
    unsigned int fileOffsetToPixelArray;
};

struct DIBHeader
{
    unsigned int unused_00;
    unsigned int width;
    unsigned int height;
    unsigned short unused_01;
    unsigned short bpp; // bits per pixel (24,32)
};
#pragma pack(pop)

static unsigned char *loadFile(const char *filename, size_t *fileSizeOut)
{
    unsigned char *data = nullptr;

    // Open file
    std::ifstream file(filename, std::ios_base::binary);
    if (!file.is_open())
    {
        printf("Error could not open file '%s'\n", filename);
        return nullptr;
    }

    // Read filesize
    file.seekg(0, std::ios_base::end);
    *fileSizeOut = file.tellg();
    file.seekg(0, std::ios_base::beg);

    // Allocate memory
    data = new unsigned char[*fileSizeOut];
    file.read((char *)data, *fileSizeOut);

    file.close();

    return data;
}

unsigned char* utils::loadBMP(const char *filename, int *widthOut, int *heightOut, int* pixelSize)
{
    size_t filesize;
    const unsigned char* data = loadFile(filename, &filesize);
    if (!data) return nullptr;
    if (filesize < (sizeof(BMPHeader)+sizeof(DIBHeader)))
    {
        printf("Error invalid file size '%s'\n", filename);
        return nullptr;
    }
    BMPHeader header1;
    DIBHeader header2;
    memcpy(&header1,data,sizeof(BMPHeader));
    memcpy(&header2,data+sizeof(BMPHeader),sizeof(DIBHeader));
    if (header1.signature[0] != 'B' || header1.signature[1] != 'M')
    {
        printf("Error corrupted file '%s'\n", filename);
        return nullptr;
    }
    if (header1.fileSize != filesize)
    {
        printf("Warning invalid size '%s': %u exepted, %zu found\n", filename, header1.fileSize, filesize);
    }
    if (widthOut) *widthOut = header2.width;
    if (heightOut) *heightOut = header2.height;
    if (header2.bpp != 24 && header2.bpp != 32 && header2.bpp != 8)
    {
        printf("Warning unsuported pixel size '%s': %u\n", filename, header2.bpp);
    }
    if (pixelSize) *pixelSize = header2.bpp;
    size_t dataSize = header2.width*header2.height*header2.bpp/8;
    unsigned char* dataOut = new unsigned char[(size_t)(header2.width)*header2.height*4];
    const unsigned char* dataPointer = data+header1.fileOffsetToPixelArray;
    if (header2.bpp == 24)
    {
        for (size_t i = 0; i < dataSize/3; i++)
        {
            dataOut[i*4] = dataPointer[i*3+2];
            dataOut[i*4+1] = dataPointer[i*3+1];
            dataOut[i*4+2] = dataPointer[i*3];
            dataOut[i*4+3] = 255u;
        }
    }
    else if (header2.bpp == 32)
    {
        for (size_t i = 0; i < dataSize/4; i++)
        {
            dataOut[i*4] = dataPointer[i*4+2];
            dataOut[i*4+1] = dataPointer[i*4+1];
            dataOut[i*4+2] = dataPointer[i*4];
            dataOut[i*4+3] = dataPointer[i*4+3];
        }
    }
    else
    {
        memcpy(dataOut,dataPointer,dataSize);
    }
    delete[] data;
    printf("Loaded file '%s' (%d bits per pixel)\n", filename, header2.bpp);
    return dataOut;
}