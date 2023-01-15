
#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>

#include <imgui.h>
#include <maths.hpp>
#include <Renderer.hpp>
#include <Clipper.hpp>
#include <msaaSampling.hpp>
#include <Shader.hpp>

Renderer::Renderer(UChar4D *p_colorBuffer, float *p_depthBuffer, const uint p_width, const uint p_height)
{
    viewport = {p_width, p_height};
    colorBuffer = p_colorBuffer;
    depthBuffer = p_depthBuffer;
}
Renderer::~Renderer() {}

void Renderer::SetColor(const UChar4D &color)
{
    renderColor = color;
}

void Renderer::SetProjection(Mat4D *p_projectionMatrix)
{
    projection = Mat4D(*p_projectionMatrix);
}

void Renderer::SetView(Mat4D *p_viewMatrix)
{
    view = Mat4D(*p_viewMatrix);
}

void Renderer::SetModel(Mat4D *p_modelMatrix)
{
    model = Mat4D(*p_modelMatrix);
}

void Renderer::SetTexture(const int &index)
{
    textureIndex = index;
}

void Renderer::DrawLine(const Vec3D &p0, const Vec3D &p1, const UChar4D &color)
{
    Int2D a = Int2D(math::cut(p0.x,0,viewport.width-1),math::cut(p0.y,0,viewport.height-1));
    Int2D b = Int2D(math::cut(p1.x,0,viewport.width-1),math::cut(p1.y,0,viewport.height-1));
    math::drawLine(colorBuffer, viewport.width, msaa, a, b, color);
}

void Renderer::CalculateVertices()
{
    for (int i = 0; i < 3; i++)
    {
        tmpVertices[i] = (model * tmpVertices[i]);
        tmpNormals[i] = (model * Vec4D(tmpNormals[i], 0)).getVector().unitVector();
        oldVertices[i] = tmpVertices[i].getVector();
        tmpVertices[i] = (view * tmpVertices[i]);
        tmpDepth[i] = tmpVertices[i].z;
        tmpVertices[i] = (projection * tmpVertices[i]);
        tmpVertices[i] = tmpVertices[i].homogenize();
    }
    VerticesData data;
    for (int i = 0; i < 6; i++)
    {
        data.vertices[i] = tmpVertices[i];
        data.normals[i] = tmpNormals[i];
        data.textureUVs[i] = tmpTextureUVs[i];
        data.colorUVs[i] = tmpColorUVs[i];
        data.tmpDepth[i] = tmpDepth[i];
        data.oldVertices[i] = oldVertices[i];
    }
    data.verticeCount = polySize;
    int clipped[3] = {0,0,0};
    for (int j = 0; j < 6; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            clipped[i] = clipped[i] & (0b111111 ^ (1 << j));
            if (j % 2 == 1)
            {
                if (tmpVertices[i][j / 2] < -1)
                    clipped[i] = clipped[i] | (1 << j);
            }
            else
            {
                if (tmpVertices[i][j / 2] > 1)
                    clipped[i] = clipped[i] | (1 << j);
            }
        }
    }
    if (clipped[0] & clipped[1] & clipped[2])
    {
        polySize = 0;
        return;
    }
    for (int i = 0; i < 3; i++)
    {
        if (tmpVertices[i].z < -1 || tmpVertices[i].z > 1)
        {
            polySize = 0;
            return;
        }
    }

    // Apply clipping.
    Clipper::clip(data, Vec3D(-1, -1, 0), Vec3D(-1, 1, 0));
    Clipper::clip(data, Vec3D(1, 1, 0), Vec3D(1, -1, 0));
    Clipper::clip(data, Vec3D(1, -1, 0), Vec3D(-1, -1, 0));
    Clipper::clip(data, Vec3D(-1, 1, 0), Vec3D(1, 1, 0));
    for (int i = 0; i < 6; i++)
    {
        tmpVertices[i] = data.vertices[i];
        tmpNormals[i] = data.normals[i];
        tmpTextureUVs[i] = data.textureUVs[i];
        tmpColorUVs[i] = data.colorUVs[i];
        tmpDepth[i] = data.tmpDepth[i];
        oldVertices[i] = data.oldVertices[i];
    }
    polySize = data.verticeCount;

    for (int k = 0; k < polySize; k++)
    {
        tmpPoints[k] = tmpVertices[k].getVector();
        tmpPoints[k] = Vec3D((int)((tmpPoints[k].x+1)*viewport.width/2), (int)((tmpPoints[k].y+1)*viewport.height/2), (tmpDepth[k]));
    }

    if (correctPerspective)
    {
        for (int i = 0; i < polySize; i++)
        {
            tmpTextureUVs[i] = tmpTextureUVs[i] / tmpPoints[i].z;
            tmpColorUVs[i] = tmpColorUVs[i] / tmpPoints[i].z;
            tmpPoints[i].z = 1 / tmpPoints[i].z;
        }
    }
}

template<Vec4D (*func)(
        const Texture *tex,
        const std::vector<Light>& lights,
        Vec3D vLight[3],
        int index[3],
        bool correctPerspective,
        bool perPixelLight,
        bool bilinear,
        float depth,
        float delta,
        Int3D& w,
        TextureUV tmpTextureUVs[3],
        ColorUV tmpColorUVs[3],
        Vec3D& pos3,
        Vec3D& normal,
        Vec3D& cameraPos,
        float deltaTime,
        float rng)>
void Renderer::DrawFace(const std::vector<Texture> &textures, const std::vector<Light> &lights, const int &a, const int &b, const int &c, bool bilinear)
{
    int index[3] = {a, b, c};
    // Draw triangle wireframe
    size_t minY = math::min(math::min(tmpPoints[index[0]].y,tmpPoints[index[1]].y),tmpPoints[index[2]].y);
    size_t maxY = math::max(math::max(tmpPoints[index[0]].y,tmpPoints[index[1]].y),tmpPoints[index[2]].y);
    size_t minX = math::min(math::min(tmpPoints[index[0]].x,tmpPoints[index[1]].x),tmpPoints[index[2]].x);
    size_t maxX = math::max(math::max(tmpPoints[index[0]].x,tmpPoints[index[1]].x),tmpPoints[index[2]].x);
    if (WireFrame)
    {
        DrawLine(tmpPoints[index[1]], tmpPoints[index[2]], lineColor);
        DrawLine(tmpPoints[index[2]], tmpPoints[index[0]], lineColor);
        DrawLine(tmpPoints[index[0]], tmpPoints[index[1]], lineColor);
    }
    else
    {

        float delta = math::edgeFunction(Vec2D(tmpPoints[a].x, tmpPoints[a].y), Vec2D(tmpPoints[b].x, tmpPoints[b].y), Vec2D(tmpPoints[c].x, tmpPoints[c].y));
        if (delta <= 0)
            return; // Backface culling

        // Lights values
        Vec3D vLight[3];
        if (!perPixelLight)
        {
            for (size_t i = 0; i < lights.size(); i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    vLight[j] = vLight[j] + lights[i].getLight(oldVertices[index[j]], cameraPos, tmpNormals[index[j]]);
                }
            }
            for (int j = 0; j < 3; j++)
            {
                vLight[j] = Vec3D(math::cut(vLight[j].x, 0, 1), math::cut(vLight[j].y, 0, 1), math::cut(vLight[j].z, 0, 1));
            }
        }

        Vec3D A;
        Vec3D B;
        Vec3D row;
        Vec2D pos = Vec2D(minX + 0.5f, minY + 0.5f);
        for (int i = 0; i < 3; i++)
        {
            A[i] = (tmpPoints[index[(i + 1) % 3]].y - tmpPoints[index[(i + 2) % 3]].y) / 16;
            B[i] = (tmpPoints[index[(i + 2) % 3]].x - tmpPoints[index[(i + 1) % 3]].x) / 16;
            row[i] = math::edgeFunction(pos, Int2D(tmpPoints[index[(i + 1) % 3]].x, tmpPoints[index[(i + 1) % 3]].y), Int2D(tmpPoints[index[(i + 2) % 3]].x, tmpPoints[index[(i + 2) % 3]].y));
        }
        // Multi thread rendering
#pragma omp parallel for
        for (size_t y = minY; y <= maxY; y++)
        {
            if (y >= viewport.height) continue;
            for (size_t x = minX; x <= maxX; x++)
            {
                if (x >= viewport.width) continue;
                int subPixels = 0;
                char subCount = 0;
                float subDepth[16];
                for (int i = 0; i < msaa * msaa; i++)
                {
                    Vec3D w = row - B * ((y - minY) * 16 + samplePoints[msaa-1][i].y) - A * ((x - minX) * 16 + samplePoints[msaa-1][i].x);
                    size_t pIndex = (y * viewport.width + x) * msaa * msaa + i;
                    if (w[0] < 0 || w[1] < 0 || w[2] < 0) continue;
                    float depth = 0;
                    Vec3D pos3;
                    Vec3D normal;
                    for (int k = 0; k < 3; k++)
                    {
                        depth -= (w[k] / delta) * (tmpPoints[index[k]].z);
                        if (perPixelLight)
                        {
                            pos3 = pos3 + oldVertices[index[k]] * (w[k] / delta);
                            normal = normal + tmpNormals[index[k]] * (w[k] / delta);
                        }
                    }
                    if (correctPerspective)
                    {
                        if (depth == 0) depth = 0.0001f;
                        depth = 1 / depth;
                    }
                    if (depth >= depthBuffer[pIndex]) continue;
                    subDepth[i] = depth;
                    subPixels = subPixels | (1 << i);
                    subCount++;
                }
                if (subCount)
                {

                    Int3D w = row - B * (y - minY) * 16 - A * (x - minX) * 16;
                    size_t pIndex = y * viewport.width + x;

                    float depth = 0;
                    Vec3D pos3;
                    Vec3D normal;
                    for (int k = 0; k < 3; k++)
                    {
                        depth -= (w[k] / delta) * (tmpPoints[index[k]].z);
                        if (perPixelLight)
                        {
                            pos3 = pos3 + oldVertices[index[k]] * (w[k] / delta);
                            normal = normal + tmpNormals[index[k]] * (w[k] / delta);
                        }
                    }
                    if (correctPerspective)
                    {
                        if (depth == 0) depth = 0.0001f;
                        depth = 1 / depth;
                    }
                    if (textureIndex != -1)
                    {
                        const Texture *tex = &textures[textureIndex];
                        Vec4D tmpColor = func(tex,lights,vLight,index,correctPerspective,perPixelLight,bilinear,depth,delta,w,tmpTextureUVs,tmpColorUVs,pos3,normal,cameraPos, deltaTime, rngState);
                        UChar4D color = UChar4D(roundf(tmpColor.x * renderColor.r), roundf(tmpColor.y * renderColor.g), roundf(tmpColor.z * renderColor.b), math::cut(roundf(tmpColor.w * renderColor.a), 0, 255));
                        if (color.a == 255u)
                        {
                            for (int i = 0; i < msaa * msaa; i++)
                            {
                                if ((subPixels & (1 << i)))
                                {
                                    depthBuffer[pIndex * msaa * msaa + i] = subDepth[i];
                                    colorBuffer[pIndex * msaa * msaa + i] = color;
                                }
                            }
                        }
                        else if (color.a != 0u)
                        {
                            for (int i = 0; i < msaa * msaa; i++)
                            {
                                if ((subPixels & (1 << i)))
                                {
                                    Vec4D colA = Vec4D(color);
                                    Vec4D colB = colorBuffer[pIndex * msaa * msaa + i];
                                    float alpha = colA.w + colB.w * (1 - colA.w);
                                    colorBuffer[pIndex * msaa * msaa + i] = ((colA * colA.w + colB * colB.w * (1 - colA.w)) * (1 / alpha));
                                }
                            }
                        }
                    }
                    else
                    {
                        Vec4D color;
                        for (int k = 0; k < 3; k++)
                            color = color + tmpColorUVs[index[k]] * (w[k] / delta);
                        if (correctPerspective)
                            color = color * -depth;
                        Vec4D tmpColor;

                        if (!perPixelLight)
                        {
                            for (int i = 0; i < 3; i++)
                            {
                                tmpColor = tmpColor + color * Vec4D(vLight[i]) * (w[i] / delta);
                            }
                        }
                        else
                        {
                            for (size_t i = 0; i < lights.size(); i++)
                            {
                                tmpColor = tmpColor + color * lights[i].getLight(pos3, cameraPos, normal);
                            }
                            tmpColor = Vec4D(math::cut(tmpColor.x, 0, 1), math::cut(tmpColor.y, 0, 1), math::cut(tmpColor.z, 0, 1), math::cut(tmpColor.w, 0, 1));
                        }
                        UChar4D colorF = UChar4D(roundf(tmpColor.x * renderColor.r), roundf(tmpColor.y * renderColor.g), roundf(tmpColor.z * renderColor.b), math::cut(roundf(tmpColor.w * renderColor.a), 0, 255));
                        if (colorF.a == 255u)
                        {
                            for (int i = 0; i < msaa * msaa; i++)
                            {
                                if ((subPixels & (1 << i)))
                                {
                                    depthBuffer[pIndex * msaa * msaa + i] = subDepth[i];
                                    colorBuffer[pIndex * msaa * msaa + i] = colorF;
                                }
                            }
                        }
                        else if (colorF.a != 0u)
                        {
                            for (int i = 0; i < msaa * msaa; i++)
                            {
                                if ((subPixels & (1 << i)))
                                {
                                    Vec4D colA = Vec4D(colorF);
                                    Vec4D colB = colorBuffer[pIndex * msaa * msaa + i];
                                    float alpha = colA.w + colB.w * (1 - colA.w);
                                    colorBuffer[pIndex * msaa * msaa + i] = ((colA * colA.w + colB * colB.w * (1 - colA.w)) * (1 / alpha));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    TriangleDraw++;
}

void Renderer::updateBuffers(UChar4D *p_colorBuffer, float *p_depthBuffer, uint p_width, uint p_height)
{
    viewport.width = p_width;
    viewport.height = p_height;
    colorBuffer = p_colorBuffer;
    depthBuffer = p_depthBuffer;
}

void Renderer::DrawTriangle(const triangleData &data, const std::vector<Light> &lights, bool bilinear)
{
    polySize = 3;
    for (size_t i = 0; i < 3; i++)
    {
        tmpVertices[i] = data.vertices[data.tri.vertices[i]];
        tmpNormals[i] = data.normals[data.tri.normals[i]];
        tmpTextureUVs[i] = data.textureUVs[data.tri.textureUVs[i]];
        tmpColorUVs[i] = data.colorUVs[data.tri.colorUVs[i]];
    }
    CalculateVertices();
    if (polySize < 3)
        return;
    for (int i = 0; i < polySize - 1; i++)
    {
        // Shader rendering
        switch (shaderselected)
        {
        case 1:
            DrawFace<shader::waveshader>(data.textures, lights, 0, i, i + 1, bilinear);
            break;
        case 2:
            DrawFace<shader::celshader>(data.textures, lights, 0, i, i + 1, bilinear);
            break;
        case 3:
            DrawFace<shader::glitchshader>(data.textures, lights, 0, i, i + 1, bilinear);
            break;
        default:
            DrawFace<shader::defaultshader>(data.textures, lights, 0, i, i + 1, bilinear);
            break;
        }
    }
}

void Renderer::ShowImGuiControls()
{
    float color[4] = {lineColor.r / 255.0f, lineColor.g / 255.0f, lineColor.b / 255.0f, lineColor.a / 255.0f};
    ImGui::ColorEdit4("lineColor", color);
    lineColor = UChar4D(color);
}
