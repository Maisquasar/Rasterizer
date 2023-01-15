#pragma once

#include <maths.hpp>
#include <bmpLoader.hpp>
#include <string>
class Texture
{
private:
    unsigned int glTexID = 0;
    std::string name;   //The name of the texture.

    // Return the Vec4D of Nearest.
    const Vec4D getNearest(const Vec2D& pos) const;
    // Return the Vec4D of Bilinear.
    const Vec4D getBilinear(const Vec2D& pos) const;
public:
    UChar4D *textureBuffer = nullptr;
    Int2D texturesize;
    
    Texture();
    Texture(const char *filename, bool anim = false);
    ~Texture();

    void deleteTexture();

    std::string getName();
    UChar4D* getTextureBuffer();
    // Return the pixel at the given position.
    const Vec4D getPixel(Vec2D pos, const bool& bilinear = false) const;
    unsigned int getTextureID() {return glTexID;}
};
