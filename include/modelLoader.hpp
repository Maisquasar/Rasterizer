#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

#include <Model.hpp>

namespace Loader
{
    const char* loadOBJFile(const char* path, int64_t &length);
    void Loop(const char *data, const int64_t& size, std::vector<Triangle>& faces, std::vector<Vertice>& vertices, std::vector<TextureUV>& tCoord, std::vector<Normal>& normals, std::vector<ColorUV>& colors);
    //Multi thread Mesh Loader.
    void MeshLoaderThread(Mesh* result, const char* path);
    void LoadMesh(const char* path, Mesh* in, int texture);
};