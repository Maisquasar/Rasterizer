#pragma once
#include <Renderer.hpp>
#include <string>

class Mesh
{
private:
    std::string MeshName = "";
public:
    int textureId = -1;
    bool loaded = true;
    bool selected = true;
    std::vector<Vertice> vertices;
    std::vector<Normal> normals;
    std::vector<TextureUV> textureUVs;
    std::vector<ColorUV> colorUVs;
    std::vector<Triangle> triangles;

    Mesh();
    ~Mesh();

    void draw(Renderer &renderer, const std::vector<Texture>& textures, const std::vector<Light>& lights, bool bilinear);

    //Set the texture id by the given parameter.
    void SetTexture(int);
    void SetMeshName(std::string name);
    std::string getName();
    void setVisibility(bool);

    // Return the number of triangles in this mesh.
    int getNumberOfTriangles();
};
// Return the spherical coords.
Vec3D getSphericalCoords(float r, float theta, float phi);

//Create Basic figures function.
Mesh CreateNewTriangle(Vec3D a, Vec3D b, Vec3D c);
Mesh CreateNewCube(Vec4D color = {1,1,1,1});
Mesh CreateNewSphere(float radius, int lat, int lon, Vec4D color = {1,1,1,1});
