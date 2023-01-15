#include <Texture.hpp>
#include <glad/gl.h>

#define USE_STB

#ifdef USE_STB
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

Texture::Texture() {}
Texture::Texture(const char *filename, bool anim)
{
    // Bmp\png loader.
#ifdef USE_STB
    int n = 0;
    textureBuffer = reinterpret_cast<UChar4D *>(stbi_load(filename, &texturesize.x, &texturesize.y, &n, 4));
#else
    textureBuffer = reinterpret_cast<UChar4D *>(utils::loadBMP(filename, &texturesize.x, &texturesize.y, nullptr));
#endif
    glGenTextures(1, &glTexID);
    glBindTexture(GL_TEXTURE_2D, glTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturesize.x, texturesize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<unsigned char*>(textureBuffer));

    //Parse name of texture
    char c;
    bool isName = false;
    for (int i = 7; i < 64; i++)
    {
        c = filename[i];
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
}

Texture::~Texture() {}

UChar4D *Texture::getTextureBuffer()
{
    return textureBuffer;
}

void Texture::deleteTexture()
{
    #ifdef USE_STB
    stbi_image_free(textureBuffer);
    #else
    delete[] textureBuffer;
    #endif
}

const Vec4D Texture::getPixel(Vec2D pos, const bool &bilinear) const
{
    #ifdef USE_STB
    pos = Vec2D(pos.x,-pos.y);
    #endif
    if (bilinear)
        return getBilinear(pos);
    return getNearest(pos);
}

const Vec4D Texture::getNearest(const Vec2D &pos) const
{
    Int2D pixel = Int2D(math::mod(pos.x * (texturesize.x), texturesize.x), math::mod(pos.y * (texturesize.y), texturesize.y));
    return Vec4D(textureBuffer[pixel.y * texturesize.x + pixel.x]);
}

const Vec4D Texture::getBilinear(const Vec2D &pos) const
{
    Vec2D texPos = Vec2D(math::mod(pos.x * texturesize.x - 0.5f, texturesize.x), math::mod(pos.y * texturesize.y - 0.5f, texturesize.y));
    Vec2D delta = Vec2D(texPos.x - static_cast<int>(texPos.x), texPos.y - static_cast<int>(texPos.y));
    Int2D px1 = Int2D(texPos);
    Int2D px2 = Int2D(math::mod(pos.x * texturesize.x + 0.5f, texturesize.x), math::mod(pos.y * texturesize.y + 0.5f, texturesize.y));
    Vec4D color11 = Vec4D(textureBuffer[px1.x+px1.y*texturesize.x]);
    Vec4D color12 = Vec4D(textureBuffer[px1.x+px2.y*texturesize.x]);
    Vec4D color21 = Vec4D(textureBuffer[px2.x+px1.y*texturesize.x]);
    Vec4D color22 = Vec4D(textureBuffer[px2.x+px2.y*texturesize.x]);
    Vec4D deltaX = color21 - color11;
    Vec4D deltaY = color12 - color11;
    Vec4D deltaXY = color11 + color22 - color21 - color12;
    return deltaX * delta.x + deltaY * delta.y + deltaXY * delta.x * delta.y + color11;
}

std::string Texture::getName() {return name;}