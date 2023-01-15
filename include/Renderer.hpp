#pragma once

#include <Vertices.hpp>
#include <Framebuffer.hpp>
#include <Texture.hpp>
#include <Light.hpp>

#define MAX_POLY 6
#ifdef WINDOWS
#define uint unsigned int
#endif // WINDOWS

struct Viewport
{
    uint width;
    uint height;
};

class Renderer
{
private:
    UChar4D *colorBuffer;
    float *depthBuffer;
    Viewport viewport;

    int textureIndex = -1;

    Mat4D view = Mat4D::Identity();
    Mat4D model = Mat4D::Identity();
    Mat4D projection = Mat4D::Identity();

    UChar4D lineColor = UChar4D(0x000000ff);

    Vertice tmpVertices[MAX_POLY];
    VerticeS tmpPoints[MAX_POLY];
    Normal tmpNormals[MAX_POLY];
    TextureUV tmpTextureUVs[MAX_POLY];
    ColorUV tmpColorUVs[MAX_POLY];
    VerticeS oldVertices[MAX_POLY];
    float tmpDepth[MAX_POLY];
    int polySize;
    UChar4D renderColor = UChar4D(0xffffffff);

public:
    // Color and depth buffer have to be valid until the shutdown of the renderer
    // Color buffer is RGBA, each component is a 8 bit unsigned char
    // Depth buffer is a buffer of 32bits floats
    Renderer(UChar4D *p_colorBuffer, float *p_depthBuffer, uint p_width, uint p_height);
    ~Renderer();
    int TriangleDraw = 0; // Number of triangle drawn
    Vec3D cameraPos = Vec3D();

    int msaa = 1;
    bool WireFrame = false;
    bool correctPerspective = true;
    bool perPixelLight = false;
    int shaderselected = 0;
    float deltaTime = 0; // Used for shaders
    float rngState = 0; // Also used for shaders

    void SetProjection(Mat4D *p_projectionMatrix);
    void SetView(Mat4D *p_viewMatrix);
    void SetModel(Mat4D *p_modelMatrix);
    void SetColor(const UChar4D& in);
    void SetTexture(const int& index);
    // Draw lines.
    void DrawLine(const Vec3D &p_0, const Vec3D &p_1, const UChar4D &p_color);
    // Draw one triangle.
    void DrawTriangle(const triangleData& data, const std::vector<Light>& lights, bool);

    void updateBuffers(UChar4D *p_colorBuffer, float *p_depthBuffer, uint p_width, uint p_height);
    //render Line color parameters in Ui.
    void ShowImGuiControls();

private:
    void CalculateVertices();
    // Draw face of triangle.
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
    void DrawFace(const std::vector<Texture>& textures, const std::vector<Light>& lights, const int& a, const int& b, const int& c, bool bilinear);
};