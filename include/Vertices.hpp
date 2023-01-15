#pragma once
#include <vector>
#include <Texture.hpp>
#include <maths.hpp>
#include <Triangles.hpp>

#define Vertice Vec4D
#define VerticeS Vec3D
#define Normal Vec3D
#define TextureUV Vec2D
#define ColorUV Vec4D

class Vertices
{
private:
public:
    Vec3D mPos;
    Vec3D mNormal;
    Vec4D mColor;
    Vertices();
    Vertices(Vec3D pos, Vec3D normal = {0,1,0}, Vec4D color = {1,1,1,0});
    ~Vertices();
};

//The triangle data struct.
struct triangleData
{
    const Triangle& tri;
    const std::vector<Texture>& textures;
    const std::vector<Vertice>& vertices;
    const std::vector<Normal>& normals;
    const std::vector<TextureUV>& textureUVs;
    const std::vector<ColorUV>& colorUVs;
};

//The Vertices data struct.
struct VerticesData
{
    Vertice vertices[6];
    Normal normals[6];
    TextureUV textureUVs[6];
    ColorUV colorUVs[6];
    VerticeS oldVertices[6];
    float tmpDepth[6];
    int verticeCount;
};
