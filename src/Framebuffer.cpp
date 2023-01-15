#include <cstring>

#include <Framebuffer.hpp>
#include <Shader.hpp>

Framebuffer::Framebuffer(int p_width, int p_height)
    : width(p_width)
    , height(p_height)
    , colorBuffer(p_width* p_height)
    , depthBuffer(p_width* p_height)
    , outBuffer(p_width* p_height)
    , imageBuffer(p_width* p_height)
{
    // Parameters for texture loading.
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &colorTexture);
}

void Framebuffer::Clear()
{
    // Clear color buffer
    {
        UChar4D* colors = colorBuffer.data();

        // Fill the first line with the clear color
        for (int i = 0; i < width; ++i)
            std::memcpy(&colors[i], &clearColor, sizeof(UChar4D));

        // Copy the first line onto every line
        for (int i = 1; i < height; ++i)
            std::memcpy(&colors[i * width], &colors[0], width * sizeof(UChar4D));
    }

    // Clear depth buffer (0x7f7fffff is the max positive value for a float)
    {
        float* depths = depthBuffer.data();
        unsigned int ref = 0x7f7fffff;
        for (int i = 0; i < width; ++i)
            std::memcpy(&depths[i], &ref, sizeof(ref));

        // Copy the first line onto every line
        for (int i = 1; i < height; ++i)
            std::memcpy(&depths[i * width], &depths[0], width * sizeof(float));
    }
}

template<void (*func)(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time)>
void Framebuffer::globalShader(const int& msaa)
{
    if (msaa > 1)
    {
        int64_t sizeOut = (width/msaa)*(height/msaa);
        for (int64_t i = 0; i < sizeOut; i++)
        {
            Vec4D colorOut;
            for (int j = 0; j < msaa * msaa; j++)
            {
                colorOut = colorOut + Vec4D(colorBuffer[i*msaa*msaa+j]);
            }
            colorOut = colorOut / (msaa*msaa);
            outBuffer[i] = colorOut;
        }
        func(outBuffer.data(),imageBuffer.data(),width/msaa,height/msaa,rngState, time);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width/msaa, height/msaa, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<unsigned char*>(imageBuffer.data()));
    }
    else
    {
        func(colorBuffer.data(),imageBuffer.data(),width,height,rngState, time);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<unsigned char*>(imageBuffer.data()));
    }
}

void Framebuffer::UpdateTexture(const int& msaa)
{
    switch (shader)
    {
    case 1:
        globalShader<shader::invColorGshader>(msaa);
        break;
    case 2:
        globalShader<shader::redGshader>(msaa);
        break;
    case 3:
        globalShader<shader::greenGshader>(msaa);
        break;
    case 4:
        globalShader<shader::blueGshader>(msaa);
        break;
    case 5:
        globalShader<shader::pixelGshader>(msaa);
        break;
    case 6:
        globalShader<shader::creeperGshader>(msaa);
        break;
    case 7:
        globalShader<shader::posterGshader>(msaa);
        break;
    case 8:
        globalShader<shader::gameBoyGshader>(msaa);
        break;
    case 9:
        globalShader<shader::UpsideDownGshader>(msaa);
        break;
    case 10:
        globalShader<shader::bAndWGshader>(msaa);
        break;
    case 11:
        globalShader<shader::hashGshader>(msaa);
        break;
    case 12:
        globalShader<shader::vhsGshader>(msaa);
        break;
    case 13:
        globalShader<shader::pumpkinGshader>(msaa);
        break;
    default:
        globalShader<shader::defaultGshader>(msaa);
        break;
    }
}

void Framebuffer::setSize(const Int2D& in, const int& msaa)
{
    width = in.x * msaa;
    height = in.y * msaa;
    colorBuffer.resize(width* height);
    depthBuffer.resize(width* height);
    outBuffer.resize(in.x*in.y);
    imageBuffer.resize(in.x*in.y);
}