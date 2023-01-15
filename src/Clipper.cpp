#include <Clipper.hpp>
float x_intersect(Vec3D a, Vec3D b, Vec3D c, Vec3D d)
{
    float num = (a.x * b.y - a.y * b.x) * (c.x - d.x) - (a.x - b.x) * (c.x * d.y - c.y * d.x);
    float den = (a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x);
    return num / den;
}

float y_intersect(Vec3D a, Vec3D b, Vec3D c, Vec3D d)
{
    float num = (a.x * b.y - a.y * b.x) * (c.y - d.y) - (a.y - b.y) * (c.x * d.y - c.y * d.x);
    float den = (a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x);
    return num / den;
}

float z_intersect(Vec3D a, Vec3D b, Vec3D c, Vec3D d)
{
    float num = (a.z * b.y - a.y * b.z) * (c.z - d.z) - (a.z - b.z) * (c.z * d.y - c.y * d.z);
    float den = (a.z - b.z) * (c.y - d.y) - (a.y - b.y) * (c.z - d.z);
    return num / den;
}

void interpolateVertice(VerticesData& in, VerticesData& tmp, const int& a, const int& b, const Vec3D& mid)
{
    float dec = mid.x - in.vertices[a].x;
    float delta = in.vertices[b].x - in.vertices[a].x;
    if (delta < 0.001f && delta > -0.001f)
    {
        dec = mid.y - in.vertices[a].y;
        delta = in.vertices[b].y - in.vertices[a].y;
    }
    if (delta == 0)
        delta = 0.0001f;
    dec = dec/delta;
    float depth = (1-dec)*(1/in.tmpDepth[a])+(dec)*(1/in.tmpDepth[b]);
    if (depth == 0) depth = 1;
    depth = 1/depth;
    tmp.vertices[tmp.verticeCount] = mid;
    tmp.normals[tmp.verticeCount] = ((in.normals[a]/in.tmpDepth[a])*(1-dec)+(in.normals[b]/in.tmpDepth[b])*(dec))*depth;
    tmp.colorUVs[tmp.verticeCount] = ((in.colorUVs[a]/in.tmpDepth[a])*(1-dec)+(in.colorUVs[b]/in.tmpDepth[b])*(dec))*depth;
    tmp.textureUVs[tmp.verticeCount] = ((in.textureUVs[a]/in.tmpDepth[a])*(1-dec)+(in.textureUVs[b]/in.tmpDepth[b])*(dec))*depth;
    tmp.oldVertices[tmp.verticeCount] = ((in.oldVertices[a]/in.tmpDepth[a])*(1-dec)+(in.oldVertices[b]/in.tmpDepth[b])*(dec))*depth;
    tmp.tmpDepth[tmp.verticeCount] = depth;
    tmp.verticeCount++;
}

void setVertice(VerticesData& in, VerticesData& tmp, const int& index)
{
    tmp.vertices[tmp.verticeCount] = in.vertices[index];
    tmp.normals[tmp.verticeCount] = in.normals[index];
    tmp.colorUVs[tmp.verticeCount] = in.colorUVs[index];
    tmp.textureUVs[tmp.verticeCount] = in.textureUVs[index];
    tmp.oldVertices[tmp.verticeCount] = in.oldVertices[index];
    tmp.tmpDepth[tmp.verticeCount] = in.tmpDepth[index];
    tmp.verticeCount++;
}

// This functions clips all the edges w.r.t one clip
// edge of clipping area
void Clipper::clip(VerticesData& in, Vec3D a, Vec3D b)
{
    VerticesData tmp;
    tmp.verticeCount = 0;

    // (ix,iy),(kx,ky) are the co-ordinate values of the points
    for (int i = 0; i < in.verticeCount; i++)
    {
        // i and k form a line in polygon
        int k = (i + 1) % in.verticeCount;
        float ix = in.vertices[i].x, iy = in.vertices[i].y;
        float kx = in.vertices[k].x, ky = in.vertices[k].y;
        float iz = in.vertices[i].z, kz = in.vertices[k].z;

        // Calculating position of first point
        float i_pos = (b.x - a.x) * (iy - a.y) - (b.y - a.y) * (ix - a.x);

        // Calculating position of second point
        float k_pos = (b.x - a.x) * (ky - a.y) - (b.y - a.y) * (kx - a.x);

        // Case 1 : When both points are inside
        if (i_pos < 0 && k_pos < 0)
        {
            // Only second point is added
            setVertice(in,tmp,k);
        }

        // Case 2: When only first point is outside
        else if (i_pos >= 0 && k_pos < 0)
        {
            // Point of intersection with edge and the second point is added
            Vec3D mid = {
                x_intersect(a, b, Vec3D(ix, iy, iz), Vec3D(kx, ky, kz)),
                y_intersect(a, b, Vec3D(ix, iy, iz), Vec3D(kx, ky, kz)),
                z_intersect(a, b, Vec3D(ix, iy, iz), Vec3D(kx, ky, kz)),
            };
            interpolateVertice(in,tmp,i,k,mid);
            // We now need to recalculate UVs

            setVertice(in,tmp,k);
        }

        // Case 3: When only second point is outside
        else if (i_pos < 0 && k_pos >= 0)
        {
            // Only point of intersection with edge is added
            Vec3D mid = {
                x_intersect(a, b, Vec3D(ix, iy, iz), Vec3D(kx, ky, kz)),
                y_intersect(a, b, Vec3D(ix, iy, iz), Vec3D(kx, ky, kz)),
                z_intersect(a, b, Vec3D(ix, iy, iz), Vec3D(kx, ky, kz)),
            };
            interpolateVertice(in,tmp,i,k,mid);
        }

        // Case 4: When both points are outside
        else
        {
            // No points are added
        }
    }

    // Copying new points into original array and changing the nbr of vertices
    in.verticeCount = tmp.verticeCount;
    for (int i = 0; i < in.verticeCount; i++)
    {
        in.vertices[i] = tmp.vertices[i];
        in.normals[i] = tmp.normals[i];
        in.colorUVs[i] = tmp.colorUVs[i];
        in.textureUVs[i] = tmp.textureUVs[i];
        in.oldVertices[i] = tmp.oldVertices[i];
        in.tmpDepth[i] = tmp.tmpDepth[i];
    }
}
