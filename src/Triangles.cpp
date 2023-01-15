#include <Triangles.hpp>

Triangle::Triangle(const unsigned int v[3], const unsigned int n[3], const unsigned int t[3], const unsigned int c[3])
{
    for (int i = 0; i < 3; i++)
    {
        vertices[i] = v[i];
        normals[i] = n[i];
        textureUVs[i] = t[i];
        colorUVs[i] = c[i];
    }
}
Triangle::~Triangle(){}