#include <Mesh.hpp>
Mesh::Mesh(){}

Mesh::~Mesh(){}

void Mesh::draw(Renderer &renderer, const std::vector<Texture> &textures, const std::vector<Light>& lights, bool bilinear)
{
    if (!loaded || !selected)
        return;
    if (textureId >= 0 && textures[textureId].textureBuffer != nullptr)
        renderer.SetTexture(textureId);
    else
        renderer.SetTexture(-1);
    for (unsigned int i = 0; i < triangles.size(); i++)
    {
        triangleData tmp = {triangles[i], textures, vertices, normals, textureUVs, colorUVs};
        renderer.DrawTriangle(tmp, lights, bilinear);
    }
}

Mesh CreateNewTriangle(Vec3D a, Vec3D b, Vec3D c)
{
    Mesh temp;
    temp.SetMeshName("Basic Triangle");
    unsigned int v[3] = {0, 1, 2};
    unsigned int v2[3] = {0, 2, 1};
    temp.vertices.push_back(a);
    temp.vertices.push_back(b);
    temp.vertices.push_back(c);
    temp.normals.push_back(Vec3D());
    temp.normals.push_back(Vec3D());
    temp.normals.push_back(Vec3D());
    temp.colorUVs.push_back(Vec3D(1, 0, 0));
    temp.colorUVs.push_back(Vec3D(0, 1, 0));
    temp.colorUVs.push_back(Vec3D(0, 0, 1));
    temp.textureUVs.push_back(Vec2D(1, 0));
    temp.textureUVs.push_back(Vec2D(0, 1));
    temp.textureUVs.push_back(Vec2D(1, 1));
    temp.triangles.push_back(Triangle(v, v, v, v));
    temp.triangles.push_back(Triangle(v2, v2, v2, v2));
    return temp;
}

Mesh CreateNewCube(Vec4D color)
{
    Mesh temp;
    temp.SetMeshName("Basic Cube");
    int x = 0;
    // Create all peaks
    temp.vertices.push_back(Vec3D{-0.5 , -0.5, -0.5});  // x = 0, y = 0, z = 0 0
    temp.vertices.push_back(Vec3D{-0.5 ,  0.5, -0.5});  // x = 0, y = 1, z = 0 1
    temp.vertices.push_back(Vec3D{ 0.5 ,  0.5, -0.5});  // x = 1, y = 1, z = 0 2
    temp.vertices.push_back(Vec3D{ 0.5 , -0.5, -0.5});  // x = 1, y = 0, z = 0 3
    temp.vertices.push_back(Vec3D{-0.5 , -0.5,  0.5});  // x = 0, y = 0, z = 1 4
    temp.vertices.push_back(Vec3D{-0.5 ,  0.5,  0.5});  // x = 0, y = 1, z = 1 5
    temp.vertices.push_back(Vec3D{ 0.5 ,  0.5,  0.5});  // x = 1, y = 1, z = 1 6
    temp.vertices.push_back(Vec3D{ 0.5 , -0.5,  0.5});  // x = 1, y = 0, z = 1 7
    temp.normals.push_back(Vec3D(1,0,0));
    temp.normals.push_back(Vec3D(-1,0,0));
    temp.normals.push_back(Vec3D(0,1,0));
    temp.normals.push_back(Vec3D(0,-1,0));
    temp.normals.push_back(Vec3D(0,0,1));
    temp.normals.push_back(Vec3D(0,0,-1));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
    temp.textureUVs.push_back(Vec2D(0, 0));
    temp.textureUVs.push_back(Vec2D(0, 1));
    temp.textureUVs.push_back(Vec2D(1, 1));
    temp.textureUVs.push_back(Vec2D(1, 0));

    //Quad (Back)
    unsigned int v[3] =  {(uint)x, (uint)x + 1, (uint)x + 2};
    unsigned int v2[3] = {(uint)x, (uint)x + 2, (uint)x + 3};
    uint c[3] = {3,2,1};
    uint c2[3] = {3,1,0};
    uint n[3] = {5,5,5};
    temp.triangles.push_back(Triangle{v,n,c,v});
    temp.triangles.push_back(Triangle{v2,n,c2,v2});

    //Quad (Front)
    unsigned int v3[3] = {(uint)x + 5 , (uint)x + 4, (uint)x + 6};
    unsigned int v4[3] = {(uint)x + 6 , (uint)x + 4, (uint)x + 7};
    uint c3[3] = {1,0,2};
    uint c4[3] = {2,0,3};
    uint n2[3] = {4,4,4};
    temp.triangles.push_back(Triangle{v3,n2,c3,v3});
    temp.triangles.push_back(Triangle{v4,n2,c4,v4});
    
    //Quad (Down)
    unsigned int v5[3] = {(uint)x + 4, (uint)x, (uint)x + 7};
    unsigned int v6[3] = {(uint)x + 7, (uint)x, (uint)x + 3};
    uint n3[3] = {3,3,3};
    temp.triangles.push_back(Triangle{v5,n3,c3,v5});
    temp.triangles.push_back(Triangle{v6,n3,c4,v6});

    //Quad (Up)
    unsigned int v7[3] = {(uint)x + 1, (uint)x + 5, (uint)x + 6};
    unsigned int v8[3] = {(uint)x + 1, (uint)x + 6, (uint)x + 2};
    uint c5[3] = {1,0,3};
    uint c6[3] = {1,3,2};
    uint n4[3] = {2,2,2};
    temp.triangles.push_back(Triangle{v7,n4,c5,v7});
    temp.triangles.push_back(Triangle{v8,n4,c6,v8});

    //Quad (Left)
    unsigned int v9 [3] = {(uint)x, (uint)x + 4, (uint)x + 5};
    unsigned int v10[3] = {(uint)x, (uint)x + 5, (uint)x + 1};
    uint c7[3] = {0,3,2};
    uint c8[3] = {0,2,1};
    uint n5[3] = {1,1,1};
    temp.triangles.push_back(Triangle{v9,n5,c7,v9});
    temp.triangles.push_back(Triangle{v10,n5,c8,v10});

    //Quad (Right)
    unsigned int v11[3] = {(uint)x + 6, (uint)x + 7, (uint)x + 3};
    unsigned int v12[3] = {(uint)x + 2, (uint)x + 6, (uint)x + 3};
    uint c9[3] = {2,1,3};
    uint n6[3] = {0,0,0};
    temp.triangles.push_back(Triangle{v11,n6,c5,v11});
    temp.triangles.push_back(Triangle{v12,n6,c9,v12});
    return temp;
}

Vec3D getSphericalCoords(float r, float theta, float phi)
{
    return {r * sinf(theta) * cosf(phi), r * cosf(theta), r * sinf(theta) * sinf(phi)};
}

Mesh CreateNewSphere(float radius, int lat, int lon, Vec4D color)
{
    Mesh temp;
    temp.SetMeshName("Basic Sphere");
    float r = radius;
    unsigned int a = 0;

    for (int j = 0; j < lat; ++j)
    {
        float theta0 = ((j + 0) / (float)lat) * M_PI;
        float theta1 = ((j + 1) / (float)lat) * M_PI;

        for (int i = 0; i < lon; ++i)
        {
            float phi0 = ((i + 0) / (float)lon) * 2.f * M_PI;
            float phi1 = ((i + 1) / (float)lon) * 2.f * M_PI;

            Vec3D c0 = getSphericalCoords(r, theta0, phi0);
            Vec3D c1 = getSphericalCoords(r, theta0, phi1);
            Vec3D c2 = getSphericalCoords(r, theta1, phi1);
            Vec3D c3 = getSphericalCoords(r, theta1, phi0);

            temp.vertices.push_back(c0);
            temp.normals.push_back(c0.unitVector());
            temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
            temp.textureUVs.push_back(Vec2D(1, 1));

            temp.vertices.push_back(c2);
            temp.normals.push_back(c2.unitVector());
            temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
            temp.textureUVs.push_back(Vec2D(0, 0));

            temp.vertices.push_back(c1);
            temp.normals.push_back(c1.unitVector());
            temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
            temp.textureUVs.push_back(Vec2D(0, 1));

            temp.vertices.push_back(c3);
            temp.normals.push_back(c3.unitVector());
            temp.colorUVs.push_back(Vec4D(color.x, color.y, color.z, color.w));
            temp.textureUVs.push_back(Vec2D(1, 0));

            unsigned int v[3] = {a + 1, a, a + 2};
            unsigned int v2[3] = {a, a + 1, a + 3};

            temp.triangles.push_back(Triangle(v, v, v, v));
            temp.triangles.push_back(Triangle(v2, v2, v2, v2));
            a = a + 4;
        }
    }
    return temp;
}


void Mesh::SetMeshName(std::string name)
{
    MeshName = name;
}

void Mesh::SetTexture(int id)
{
    textureId = id;
}

int Mesh::getNumberOfTriangles()
{
    return triangles.size();
}

std::string Mesh::getName()
{
    return MeshName;
}
void Mesh::setVisibility(bool out)
{
    selected = out;
}