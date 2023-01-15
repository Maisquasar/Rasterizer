#pragma once
#include <vector>
#include <maths.hpp>
#include <Texture.hpp>
#include <Light.hpp>
#include <Vertices.hpp>

namespace shader
{
    void setPumpkinTexture(Texture* tex);
    void incrementDec(float deltaTime);
    // Default fragment shader, will render lixel lightning based on vertices or interpolated position if perPixelLight is true
    Vec4D defaultshader(
        const Texture* tex,
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
        float rng);

    // Waves fragment shader, will deform texture and normals to create a water-like effect
    Vec4D waveshader(
        const Texture* tex,
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
        float rng);

    // Cel-Shading fragment shader, will reduce the shading tints in order to create a more flat lightning
    Vec4D celshader(
        const Texture* tex,
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
        float rng);

    // Glitch fragment shader, simulates a glitched render
    Vec4D glitchshader(
        const Texture* tex,
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
        float rng);

    void defaultGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void invColorGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void redGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void greenGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void blueGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void pixelGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void creeperGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void posterGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height);

    void posterGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void gameBoyGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void vhsGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void UpsideDownGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void bAndWGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void hashGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);

    void pumpkinGshader(
        UChar4D* bufferIn,
        UChar4D* bufferOut,
        int width,
        int height,
        float rng,
        float time);
} // namespace shader
