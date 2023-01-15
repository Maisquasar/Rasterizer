#include <Shader.hpp>

static Texture* pumpkinTex;
static Vec2D dec = Vec2D();

Vec4D shader::defaultshader(const Texture *tex,
                            const std::vector<Light> &lights,
                            Vec3D vLight[3],
                            int index[3],
                            bool correctPerspective,
                            bool perPixelLight,
                            bool bilinear,
                            float depth,
                            float delta,
                            Int3D &w,
                            TextureUV tmpTextureUVs[3],
                            ColorUV tmpColorUVs[3],
                            Vec3D &pos3,
                            Vec3D &normal,
                            Vec3D &cameraPos,
                            float deltaTime,
                            float rng)
{
    Vec2D texturePos;
    for (int k = 0; k < 3; k++)
        texturePos = texturePos + tmpTextureUVs[index[k]] * (w[k] / delta);
    if (correctPerspective)
        texturePos = texturePos * -depth;
    Vec4D texColor = tex->getPixel(texturePos, bilinear);
    Vec4D tmpColor;
    if (!perPixelLight)
    {
        for (int i = 0; i < 3; i++)
        {
            tmpColor = tmpColor + texColor * vLight[i] * (w[i] / delta);
        }
    }
    else
    {
        for (size_t i = 0; i < lights.size(); i++)
        {
            tmpColor = tmpColor + texColor * lights[i].getLight(pos3, cameraPos, normal);
        }
        tmpColor = Vec4D(math::cut(tmpColor.x, 0, 1), math::cut(tmpColor.y, 0, 1), math::cut(tmpColor.z, 0, 1), math::cut(tmpColor.w, 0, 1));
    }
    return tmpColor;
}

Vec4D shader::waveshader(const Texture *tex,
                         const std::vector<Light> &lights,
                         Vec3D vLight[3],
                         int index[3],
                         bool correctPerspective,
                         bool perPixelLight,
                         bool bilinear,
                         float depth,
                         float delta,
                         Int3D &w,
                         TextureUV tmpTextureUVs[3],
                         ColorUV tmpColorUVs[3],
                         Vec3D &pos3,
                         Vec3D &normal,
                         Vec3D &cameraPos,
                         float deltaTime,
                         float rng)
{
    Vec2D texturePos;
    for (int k = 0; k < 3; k++)
        texturePos = texturePos + tmpTextureUVs[index[k]] * (w[k] / delta);
    if (correctPerspective)
        texturePos = texturePos * -depth;
    normal = Vec3D(normal.x + sinf(deltaTime + pos3.x * 2) * 0.3f, normal.y + sinf(deltaTime + pos3.y * 1.37f) * 0.3f, normal.z + sinf(deltaTime + pos3.z * 2.3f + 1.37f) * 0.3f);
    texturePos = Vec2D(texturePos.x + sinf(deltaTime + pos3.x * 2) * 0.017f, texturePos.y + sinf(deltaTime + pos3.z * 2.3 + 1.37f) * 0.017f) + dec/100;
    Vec4D texColor = tex->getPixel(texturePos, bilinear);
    Vec4D tmpColor;
    if (!perPixelLight)
    {
        for (int i = 0; i < 3; i++)
        {
            tmpColor = tmpColor + texColor * vLight[i] * (w[i] / delta);
        }
    }
    else
    {
        for (size_t i = 0; i < lights.size(); i++)
        {
            tmpColor = tmpColor + texColor * lights[i].getLight(pos3, cameraPos, normal);
        }
        tmpColor = Vec4D(math::cut(tmpColor.x, 0, 1), math::cut(tmpColor.y, 0, 1), math::cut(tmpColor.z, 0, 1), math::cut(tmpColor.w, 0, 1));
    }
    return tmpColor;
}

Vec4D shader::celshader(const Texture *tex,
                        const std::vector<Light> &lights,
                        Vec3D vLight[3],
                        int index[3],
                        bool correctPerspective,
                        bool perPixelLight,
                        bool bilinear,
                        float depth,
                        float delta,
                        Int3D &w,
                        TextureUV tmpTextureUVs[3],
                        ColorUV tmpColorUVs[3],
                        Vec3D &pos3,
                        Vec3D &normal,
                        Vec3D &cameraPos,
                        float deltaTime,
                        float rng)
{
    Vec2D texturePos;
    for (int k = 0; k < 3; k++)
        texturePos = texturePos + tmpTextureUVs[index[k]] * (w[k] / delta);
    if (correctPerspective)
        texturePos = texturePos * -depth;
    Vec4D texColor = tex->getPixel(texturePos, bilinear);
    Vec4D tmpColor;
    if (!perPixelLight)
    {
        for (int i = 0; i < 3; i++)
        {
            tmpColor = tmpColor + texColor * vLight[i] * (w[i] / delta);
        }
    }
    else
    {
        float delta = normal.unitVector().dotProduct((pos3 - cameraPos).unitVector());
        if (delta < 0)
            delta = -delta;
        for (size_t i = 0; i < lights.size(); i++)
        {
            tmpColor = tmpColor + texColor * lights[i].getLight(pos3, cameraPos, normal);
        }
        if (delta < 0.7f)
        {
            float mult = 0.79f;
            if (delta < 0.45f)
            {
                mult = 0.46f;
                if (delta < 0.2f)
                {
                    mult = 0.09f;
                }
            }
            tmpColor = tmpColor * mult;
            tmpColor.w = 1;
        }
        tmpColor = Vec4D(math::cut(tmpColor.x, 0, 1), math::cut(tmpColor.y, 0, 1), math::cut(tmpColor.z, 0, 1), math::cut(tmpColor.w, 0, 1));
    }
    return tmpColor;
}

Vec4D shader::glitchshader(const Texture *tex,
                           const std::vector<Light> &lights,
                           Vec3D vLight[3],
                           int index[3],
                           bool correctPerspective,
                           bool perPixelLight,
                           bool bilinear,
                           float depth,
                           float delta,
                           Int3D &w,
                           TextureUV tmpTextureUVs[3],
                           ColorUV tmpColorUVs[3],
                           Vec3D &pos3,
                           Vec3D &normal,
                           Vec3D &cameraPos,
                           float deltaTime,
                           float rng)
{
    Vec2D texturePos;
    for (int k = 0; k < 3; k++)
        texturePos = texturePos + tmpTextureUVs[index[k]] * (w[k] / delta);
    if (correctPerspective)
        texturePos = texturePos * -depth;
    if (rng < 0.09f)
        texturePos = texturePos + Vec2D(rng / 0.09f, rng / 0.09f + 0.137f);
    if (rng > 0.975f)
    {
        float t = math::mod((rng - 0.975f) / 0.025f * texturePos.y * 10, 1);
        texturePos = Vec2D(0.1f, (t > 0.995f) ? 0.1f : 0.9f);
    }
    texturePos = Vec2D(math::mod(texturePos.x, 1), math::mod(texturePos.y, 1));
    if ((texturePos.x < 0.5f) ^ (texturePos.y < 0.5f))
        return Vec4D(0, 0, 0);
    else
        return Vec4D(0.5f, 0, 1.0f);
}

void shader::defaultGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < width * height; i++)
    {
        bufferOut[i] = bufferIn[i];
    }
}

void shader::invColorGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < width * height; i++)
    {
        UChar4D tmp = bufferIn[i];
        bufferOut[i] = UChar4D(255u - tmp.r, 255u - tmp.g, 255u - tmp.b, tmp.a);
    }
}

void shader::redGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < width * height; i++)
    {
        Vec4D tmp = Vec4D(bufferIn[i]);
        float delta = tmp.x + tmp.y + tmp.z;
        tmp = Vec4D((tmp.x - delta / 3 > 0.3f ? tmp.x * 1.5f : delta / 3), delta / 3, delta / 3);

        bufferOut[i] = UChar4D(tmp);
    }
}

void shader::greenGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < width * height; i++)
    {
        Vec4D tmp = Vec4D(bufferIn[i]);
        float delta = tmp.x + tmp.y + tmp.z;
        tmp = Vec4D(delta / 3, (tmp.y - delta / 3 > 0.3f ? tmp.y * 1.5f : delta / 3), delta / 3);

        bufferOut[i] = UChar4D(tmp);
    }
}

void shader::blueGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < width * height; i++)
    {
        Vec4D tmp = Vec4D(bufferIn[i]);
        float delta = tmp.x + tmp.y + tmp.z;
        tmp = Vec4D(delta / 3, delta / 3, (tmp.z - delta / 3 > 0.3f ? tmp.z * 1.5f : delta / 3));

        bufferOut[i] = UChar4D(tmp);
    }
}

#define PIXEL_SIZE 4
void shader::pixelGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t x = 0; x < width / PIXEL_SIZE; x++)
        for (int64_t y = 0; y < height / PIXEL_SIZE; y++)
        {
            UChar4D tmp = bufferIn[(y * PIXEL_SIZE + PIXEL_SIZE / 2) * width + x * PIXEL_SIZE + PIXEL_SIZE / 2];
            for (int i = 0; i < PIXEL_SIZE; i++)
                for (int j = 0; j < PIXEL_SIZE; j++)
                {
                    bufferOut[(y * PIXEL_SIZE + j) * width + x * PIXEL_SIZE + i] = tmp;
                }
        }
}

void shader::creeperGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t x = 0; x < width / PIXEL_SIZE * 2; x++)
        for (int64_t y = 0; y < height / PIXEL_SIZE * 2; y++)
        {
            Vec4D tmp = Vec4D(bufferIn[(y * PIXEL_SIZE / 2 + PIXEL_SIZE / 4) * width + x * PIXEL_SIZE / 2 + PIXEL_SIZE / 4]);
            float gray = (tmp.x + tmp.y + tmp.z) / 3;
            tmp = Vec4D{0, gray, 0, tmp.w};
            for (int i = 0; i < PIXEL_SIZE / 2; i++)
                for (int j = 0; j < PIXEL_SIZE / 2; j++)
                {
                    bufferOut[(y * PIXEL_SIZE / 2 + j) * width + x * PIXEL_SIZE / 2 + i] = UChar4D(tmp);
                }
        }
}

#define LEVEL 3
void shader::posterGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < width * height; i++)
    {
        UChar4D tmp = bufferIn[i];
        tmp = tmp * ((LEVEL / 256.0f)) * (256 / LEVEL) + UChar4D(256 / (LEVEL * 2), 256 / (LEVEL * 2), 256 / (LEVEL * 2), 0);
        bufferOut[i] = tmp;
    }
}

void shader::gameBoyGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t x = 0; x < width / PIXEL_SIZE; x++)
        for (int64_t y = 0; y < height / PIXEL_SIZE; y++)
        {
            UChar4D tmp = bufferIn[(y * PIXEL_SIZE + PIXEL_SIZE / 2) * width + x * PIXEL_SIZE + PIXEL_SIZE / 2];
            tmp = tmp * ((LEVEL / 256.0f)) * (256 / LEVEL) + UChar4D(256 / (LEVEL * 2), 256 / (LEVEL * 2), 256 / (LEVEL * 2), 0);
            for (int i = 0; i < PIXEL_SIZE; i++)
                for (int j = 0; j < PIXEL_SIZE; j++)
                {
                    bufferOut[(y * PIXEL_SIZE + j) * width + x * PIXEL_SIZE + i] = tmp;
                }
        }
}

void shader::vhsGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t x = 0; x < width / PIXEL_SIZE; x++)
        for (int64_t y = 0; y < height / PIXEL_SIZE; y++)
        {
            Vec4D tmp = bufferIn[(y * PIXEL_SIZE + PIXEL_SIZE / 2) * width + x * PIXEL_SIZE + PIXEL_SIZE / 2];
            tmp = tmp * Vec4D(1.1f, 1.13f, 1.28f, 1);
            Vec4D prevX = (x > 2 ? Vec4D(bufferIn[(y * PIXEL_SIZE + PIXEL_SIZE / 2) * width + (x - 1) * PIXEL_SIZE + PIXEL_SIZE / 2]) : Vec4D());
            Vec4D prevY = (y > 0 ? Vec4D(bufferIn[((y - 1) * PIXEL_SIZE + PIXEL_SIZE / 2) * width + x * PIXEL_SIZE + PIXEL_SIZE / 2]) : Vec4D());
            prevX = prevX * Vec4D(1.1f, 1.13f, 1.28f, 1);
            prevY = prevY * Vec4D(1.1f, 1.13f, 1.28f, 1);
            Vec4D nextX = (x < (width / PIXEL_SIZE - 3) ? Vec4D(bufferIn[(y * PIXEL_SIZE + PIXEL_SIZE / 2) * width + (x + 1) * PIXEL_SIZE + PIXEL_SIZE / 2]) : Vec4D());
            Vec4D nextY = (y < (height / PIXEL_SIZE - 1) ? Vec4D(bufferIn[((y + 1) * PIXEL_SIZE + PIXEL_SIZE / 2) * width + x * PIXEL_SIZE + PIXEL_SIZE / 2]) : Vec4D());
            nextX = nextX * Vec4D(1.1f, 1.13f, 1.28f, 1);
            nextY = nextY * Vec4D(1.1f, 1.13f, 1.28f, 1);
            if (x < 2 || x > (width / PIXEL_SIZE - 3) || y < 1 || y > (height / PIXEL_SIZE - 2))
                tmp = Vec4D();
            else if (x < 3 || x > (width / PIXEL_SIZE - 4))
                tmp = Vec4D(0, 0.9f, 0, 1.0f);
            float t = math::mod((powf((x + y * width) / (width * height * 0.114f) + rng, 5)), 1) * ((y > (height / PIXEL_SIZE / 2) ? (float)(y - height / PIXEL_SIZE / 2) / (height / PIXEL_SIZE / 2) : 0));
            for (int i = 0; i < PIXEL_SIZE; i++)
            {
                int u = i + (x > 1 && x < (width / PIXEL_SIZE - 2) ? (int)(1.1 * sinf(time * 150 + y / 20.f)) : 0);
                for (int j = 0; j < PIXEL_SIZE; j++)
                {
                    if ((y > 0) && (y < (height / PIXEL_SIZE - 1)) && (x > 1) && (x < (width / PIXEL_SIZE - 2)))
                    {
                        if (j == 0)
                        {
                            bufferOut[(y * PIXEL_SIZE + j) * width + x * PIXEL_SIZE + u] = (t > 0.8f && x > 2 && x < (width / PIXEL_SIZE - 3)) ? UChar4D((tmp * 2 + prevY) / 6.5f) : UChar4D((tmp * 2 + prevY) / 3);
                            continue;
                        }
                        if (j == PIXEL_SIZE - 1)
                        {
                            bufferOut[(y * PIXEL_SIZE + j) * width + x * PIXEL_SIZE + u] = (t > 0.8f && x > 2 && x < (width / PIXEL_SIZE - 3)) ? UChar4D((tmp + nextY) / 5.9f) : UChar4D((tmp + nextY) / 2);
                            continue;
                        }
                        if (i == 0)
                        {
                            bufferOut[(y * PIXEL_SIZE + j) * width + x * PIXEL_SIZE + u] = (t > 0.8f && x > 2 && x < (width / PIXEL_SIZE - 3)) ? UChar4D((tmp * 2 + prevX) / 6.5f) : UChar4D((tmp * 2 + prevX) / 3);
                            continue;
                        }
                        if (i == PIXEL_SIZE - 1)
                        {
                            bufferOut[(y * PIXEL_SIZE + j) * width + x * PIXEL_SIZE + u] = (t > 0.8f && x > 2 && x < (width / PIXEL_SIZE - 3)) ? UChar4D((tmp + nextX) / 5.9f) : UChar4D((tmp + nextX) / 2);
                            continue;
                        }
                    }
                    bufferOut[(y * PIXEL_SIZE + j) * width + x * PIXEL_SIZE + u] = (t > 0.8f && x > 2 && x < (width / PIXEL_SIZE - 3)) ? UChar4D() : UChar4D(tmp);
                }
            }
        }
}

void shader::UpsideDownGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < height; i++)
    {
        for (int64_t j = 0; j < width; j++)
        {
            UChar4D tmp = bufferIn[i * width + j];
            bufferOut[(height - i - 1) * width + (j)] = tmp;
        }
    }
}

void shader::bAndWGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < width * height; i++)
    {
        Vec4D tmp = bufferIn[i];
        float gray = (tmp.x + tmp.y + tmp.z) / 3;
        tmp = Vec4D{gray, gray, gray, tmp.w};

        bufferOut[i] = UChar4D(tmp);
    }
}

#define DECAL 8
void shader::hashGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < height; i++)
    {
        for (int64_t j = 0; j < width; j++)
        {
            UChar4D tmp = bufferIn[i * width + j];
            if (i % 8 < 4)
            {
                tmp = bufferIn[i * width + (j + DECAL) % width];
            }
            bufferOut[i * width + j] = tmp;
        }
    }
}

void shader::pumpkinGshader(
    UChar4D *bufferIn,
    UChar4D *bufferOut,
    int width,
    int height,
    float rng,
    float time)
{
    for (int64_t i = 0; i < height; i++)
    {
        for (int64_t j = 0; j < width; j++)
        {
            Vec4D tmp = UChar4D(bufferIn[(i) * width + j]);
            Vec4D tx = UChar4D(pumpkinTex->getPixel(Vec2D((float)(j)/width,(float)(height - i - 1)/height)));
            tmp = tmp * (1-tx.w);
            tmp.w = 1.0;
            bufferOut[(i) * width + (j)] = UChar4D(tmp);
        }
    }
}

void shader::setPumpkinTexture(Texture* in)
{
    pumpkinTex = in;
}

void shader::incrementDec(float deltatime)
{
    dec = dec + Vec2D(0.5f+sinf(deltatime+0.125f),0.5f+sinf(deltatime-0.034f))*0.1f;
}