#pragma once

#include <vector>
#include <glad/gl.h>
#include <maths.hpp>

class Framebuffer
{
private:
    int width = 0;
    int height = 0;

    // In-RAM buffers
    std::vector<UChar4D> colorBuffer;
    std::vector<float> depthBuffer;
    std::vector<UChar4D> outBuffer;
    std::vector<UChar4D> imageBuffer;

    // OpenGL texture (in VRAM)
    GLuint colorTexture = 0;

public:
    //UChar4D clearColor = UChar4D(0x00c3ffff);
    UChar4D clearColor = UChar4D(0x002071ff);
    int shader = 0;
    float rngState = 0;
    float time = 0;

    Framebuffer(int width, int height);
    ~Framebuffer();

    // Clear the Framebuffer.
    void Clear();
    // Update Textures.

    void UpdateTexture(const int &msaa);

    UChar4D *GetColorBuffer() { return colorBuffer.data(); }
    float *GetDepthBuffer() { return depthBuffer.data(); }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    // Set the Framebuffer's size by the size given and the msaa value.
    void setSize(const Int2D &in, const int &msaa);

    GLuint GetColorTexture() const { return colorTexture; }

private:
    template <void (*func)(
        UChar4D *bufferIn,
        UChar4D *bufferOut,
        int width,
        int height,
        float rng,
        float time)>
    void globalShader(const int &msaa);
};