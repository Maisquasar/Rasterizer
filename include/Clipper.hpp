#pragma once

#include <maths.hpp>
#include <Vertices.hpp>

// Returns x-value of point of intersection of two lines.
float x_intersect(Vec3D a, Vec3D b, Vec3D c, Vec3D d);

// Returns y-value of point of intersection of two lines.
float y_intersect(Vec3D a, Vec3D b, Vec3D c, Vec3D d);

// Returns z-value of point of intersection of two lines.
float z_intersect(Vec3D a, Vec3D b, Vec3D c, Vec3D d);

//Interpolate vertice given.
void interpolateVertice(VerticesData& in, VerticesData& tmp, const int& a, const int& b, const Vec3D& mid);

//Set the vertice value.
void setVertice(VerticesData& in, VerticesData& tmp, const int& index);

namespace Clipper
{
    void clip(VerticesData& vertices, Vec3D a, Vec3D b);
}; // namespace Clipper.