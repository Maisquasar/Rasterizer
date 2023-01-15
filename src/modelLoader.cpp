#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <time.h>

#include <modelLoader.hpp>

#include <maths.hpp>
#include <textHelper.hpp>
#include <Sleep.hpp>

const char* Loader::loadOBJFile(const char *path, int64_t &length)
{
    std::ifstream in;
    in.open(path, std::ios::in | std::ios::ate);
    if (!in.is_open())
    {
        std::cout << "Error, cannot open file: " << path << std::endl;
        return nullptr;
    }
    std::cout << "Loading Model: " << path << std::endl;
    char *data;
    length = in.tellg();
    in.seekg(0, in.beg);

    data = new char[length];
    in.read(data, length);
    in.close();
    return data;
}

void Loader::Loop(const char *data, const int64_t& size, std::vector<Triangle>& faces, std::vector<Vertice>& vertices, std::vector<TextureUV>& tCoord, std::vector<Normal>& normals, std::vector<ColorUV>& colors)
{
    int64_t pos = 0;
    while (pos < size)
    {
        if (text::compareWord(data, pos, size, "v "))
        {
            pos = text::skipCharSafe(data, pos, size);
            float a = text::getFloat(data,pos,size);
            pos = text::skipCharSafe(data, pos, size);
            float b = text::getFloat(data,pos,size);
            pos = text::skipCharSafe(data, pos, size);
            float c = text::getFloat(data,pos,size);
            vertices.push_back(Vec3D(a, b, c));
            pos = text::endLine(data, pos, size);
        }
        else if (text::compareWord(data, pos, size, "vn "))
        {
            pos = text::skipCharSafe(data, pos, size);
            float a = text::getFloat(data,pos,size);
            pos = text::skipCharSafe(data, pos, size);
            float b = text::getFloat(data,pos,size);
            pos = text::skipCharSafe(data, pos, size);
            float c = text::getFloat(data,pos,size);
            normals.push_back(Vec3D(a, b, c));
            colors.push_back(Vec3D(a, b, c));
            pos = text::endLine(data, pos, size);
        }
        else if (text::compareWord(data, pos, size, "vt "))
        {
            pos = text::skipCharSafe(data, pos, size);
            float a = text::getFloat(data,pos,size);
            pos = text::skipCharSafe(data, pos, size);
            float b = text::getFloat(data,pos,size);
            tCoord.push_back(Vec2D(a, b));
            pos = text::endLine(data, pos, size);
        }
        else if (text::compareWord(data, pos, size, "f "))
        {
            pos = text::skipCharSafe(data, pos, size);
            int type = text::countSlash(data, pos, size);
            unsigned int v[3] = {0};
            unsigned int t[3] = {0};
            unsigned int n[3] = {0};
            if (type == -1)
            {
                v[0] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                pos += 1;
                n[0] = text::getInt(data, pos, size) -1;
                pos = text::skipCharSafe(data, pos, size);
                v[1] = text::getInt(data, pos, size) -1;
                pos = text::skipCharSafe(data, pos, size);
                pos += 1;
                n[1] = text::getInt(data, pos, size) -1;
                pos = text::skipCharSafe(data, pos, size);
                v[2] = text::getInt(data, pos, size) -1;
                pos = text::skipCharSafe(data, pos, size);
                pos += 1;
                n[2] = text::getInt(data,pos,size)-1;
            }
            else if (type == 0)
            {
                v[0] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                v[1] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                v[2] = text::getInt(data,pos,size)-1;
            }
            else if (type == 1)
            {
                v[0] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                t[0] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                v[1] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                t[1] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                v[2] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                t[2] = text::getInt(data,pos,size)-1;
            }
            else if (type == 2)
            {
                v[0] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                t[0] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                n[0] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                v[1] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                t[1] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                n[1] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                v[2] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                t[2] = text::getInt(data,pos,size)-1;
                pos = text::skipCharSafe(data, pos, size);
                n[2] = text::getInt(data,pos,size)-1;
            }
            faces.push_back(Triangle(v,n,t,n));

            pos = text::endLine(data, pos, size);
        }
        else
            pos = text::endLine(data, pos, size);
    }
}
void Loader::MeshLoaderThread(Mesh* result, const char *path)
{
    int64_t size;
    const char *data = loadOBJFile(path, size);
    if (data == nullptr) return;
    Loop(data, size, result->triangles, result->vertices, result->textureUVs, result->normals, result->colorUVs);
    result->loaded = true;
    std::cout << "Successfully loaded model: " << path << std::endl;
    delete[] data;
    return;
}

void Loader::LoadMesh(const char *path, Mesh* in, int texture)
{
    while (!in->loaded)
    {
#ifdef WINDOWS
        long long tm = (long long)(1000000000 / 20);
        nanosleep(tm);
#else
        timespec time = { 0 };
        time.tv_nsec = (long)(1000000000 / 20);
        nanosleep(&time, NULL);
#endif // WINDOWS
    }
    in->loaded = false;
    in->SetTexture(texture);

    // Parse the name of mesh
    char c;
    bool isName = false;
    std::string name;
    for (int i = 7; i < 64; i++)
    {
        c = path[i];
        if (c == '.')
        {
            break;
        }
        if (isName)
        {
            name.push_back(c);
        }
        if (c == '/')
        {
            isName = true;
        }
    }
    if (name.size() <= 0) name = "";
    in->SetMeshName(name);

    //Multi thread loading
    std::thread cThread(MeshLoaderThread, in, path);
    cThread.detach();
}

