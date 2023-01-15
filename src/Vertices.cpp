#include <Vertices.hpp>
Vertices::Vertices(){}
Vertices::Vertices(Vec3D pos, Vec3D normal, Vec4D color)
{
    mPos = pos;
    mNormal = normal;
    mColor = color;
}
Vertices::~Vertices(){}
