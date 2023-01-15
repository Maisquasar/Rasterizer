#pragma once
#include <maths.hpp>

class Triangle
{
private:
public:
    unsigned int vertices[3];
    unsigned int normals[3];
    unsigned int textureUVs[3];
    unsigned int colorUVs[3];
    Triangle(const unsigned int v[3], const unsigned int n[3], const unsigned int t[3], const unsigned int c[3]);
    ~Triangle();
};