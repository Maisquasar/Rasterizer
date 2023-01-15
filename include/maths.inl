#include <cstdio>

#include "maths.hpp"

#ifdef WINDOWS
#include <corecrt_math_defines.h>
#define __INT32_MAX__ 0x7fffffff
#endif // WINDOWS

// -----------------------   Int2D    -----------------------

inline Int2D::Int2D(const Vec2D &in) : x(in.x), y(in.y) {}

inline Int2D Int2D::operator+(const Int2D &a)
{
    Int2D res = Int2D(a.x + this->x, a.y + this->y);
    return res;
}

inline Int2D Int2D::operator-(const Int2D &a)
{
    return Int2D(x - a.x, y - a.y);
}

inline Int2D Int2D::operator*(const Int2D &a)
{
    Int2D res = Int2D(this->x * a.x, this->y * a.y);
    return res;
}

inline Int2D Int2D::operator*(const float &a)
{
    Int2D res = Int2D(this->x * a, this->y * a);
    return res;
}

inline Int2D Int2D::operator/(const float &a)
{
    if (a == 0)
        return Int2D(__INT32_MAX__, __INT32_MAX__);
    Int2D res = Int2D(x/a, y/a);
    return res;
}

inline bool Int2D::operator==(const Int2D &b)
{
    return (x == b.x && y == b.y);
}

// -----------------------   Vec2D    -----------------------

inline float Vec2D::lengthSquared()
{
    return (x * x + y * y);
}

inline float Vec2D::getLength()
{
    return sqrtf(lengthSquared());
}

inline Vec2D Vec2D::operator+(const Vec2D &a)
{
    Vec2D res = Vec2D(a.x + this->x, a.y + this->y);
    return res;
}

inline Vec2D Vec2D::operator-(const Vec2D &a)
{
    Vec2D res = Vec2D(this->x - a.x, this->y - a.y);
    return res;
}

inline Vec2D Vec2D::operator*(const Vec2D &a)
{
    Vec2D res = Vec2D(this->x * a.x, this->y * a.y);
    return res;
}

inline Vec2D Vec2D::operator*(const float &a)
{
    Vec2D res = Vec2D(this->x * a, this->y * a);
    return res;
}

inline Vec2D Vec2D::operator/(const float &a)
{
    if (a == 0.0)
        return operator*(VEC_HIGH_VALUE);
    Vec2D res = operator*(1 / a);
    return res;
}

inline bool Vec2D::operator==(const Vec2D &b)
{
    return (x == b.x && y == b.y);
}

inline bool Vec2D::isCollinearWith(Vec2D a)
{
    float res = a.x * y - a.y * x;
    return (res < VEC_COLLINEAR_PRECISION);
}

inline float Vec2D::dotProduct(Vec2D a)
{
    return (a.x * x + a.y * y);
}

inline float Vec2D::crossProduct(Vec2D a)
{
    return (x * a.y - y * a.x);
}

inline Vec2D Vec2D::unitVector()
{
    return operator/(getLength());
}

inline Vec2D Vec2D::negate()
{
    return operator*(-1);
}

inline Vec2D Vec2D::getNormal()
{
    return Vec2D(-y, x);
}

inline bool Vec2D::isIntEquivalent(Vec2D a)
{
    return ((int)x == (int)a.x && (int)y == a.y);
}

inline float Vec2D::getDistanceFromPoint(Vec2D a)
{
    float i = a.x - x;
    float j = a.y - y;
    return sqrt(i*i + j*j);
}

// -----------------------   Int3D    -----------------------

inline Int3D::Int3D(const Vec3D &in) : x(in.x), y(in.y), z(in.z) {}

inline Int3D Int3D::operator+(const Int3D &a)
{
    return Int3D(a.x + this->x, a.y + this->y, a.z + this->z);
}

inline Int3D Int3D::operator-(const Int3D &a)
{
    return Int3D(x - a.x, y - a.y, z - a.z);
}

inline Int3D Int3D::operator*(const Int3D &a)
{
    Int3D res = Int3D(this->x * a.x, this->y * a.y, this->z * a.z);
    return res;
}

inline Int3D Int3D::operator*(const float &a)
{
    Int3D res = Int3D(this->x * a, this->y * a, this->z * a);
    return res;
}

inline Int3D Int3D::operator/(const float &a)
{
    if (a == 0)
        return Int3D(__INT32_MAX__, __INT32_MAX__, __INT32_MAX__);
    Int3D res = Int3D(x/a, y/a, z/a);
    return res;
}

inline bool Int3D::operator==(const Int3D &b)
{
    return (x == b.x && y == b.y);
}

inline int& Int3D::operator[](const size_t a)
{
    return *((&x)+a);
}

// -----------------------   Vec3D    -----------------------

inline float Vec3D::lengthSquared() const
{
    return (x * x + y * y + z * z);
}

inline float Vec3D::getLength() const
{
    return sqrtf(lengthSquared());
}

inline Vec3D Vec3D::operator+(const Vec3D &a) const
{
    Vec3D res = Vec3D(a.x + this->x, a.y + this->y, a.z + this->z);
    return res;
}

inline Vec3D Vec3D::operator-(const Vec3D &a) const
{
    Vec3D res = Vec3D(this->x - a.x, this->y - a.y, this->z - a.z);
    return res;
}

inline Vec3D Vec3D::operator*(const Vec3D &a) const
{
    Vec3D res = Vec3D(this->x * a.x, this->y * a.y, this->z * a.z);
    return res;
}

inline Vec3D Vec3D::operator*(const float &a) const
{
    Vec3D res = Vec3D(this->x * a, this->y * a, this->z * a);
    return res;
}

inline Vec3D Vec3D::operator/(const float &a) const
{
    if (a == 0.0f)
        return operator*(VEC_HIGH_VALUE);
    Vec3D res = operator*(1 / a);
    return res;
}

inline bool Vec3D::operator==(const Vec3D &b) const
{
    return (x == b.x && y == b.y && z == b.z);
}

inline float& Vec3D::operator[](const size_t a)
{
    return *((&x)+a);
}

inline const float& Vec3D::operator[](const size_t a) const
{
    return *((&x)+a);
}

inline float Vec3D::dotProduct(Vec3D a)
{
    return (a.x * x + a.y * y + a.z * z);
}

inline bool Vec3D::isCollinearWith(Vec3D a)
{
    float res = this->dotProduct(a);
    return (res < VEC_COLLINEAR_PRECISION);
}

inline Vec3D Vec3D::crossProduct(Vec3D a)
{
    return Vec3D((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x));
}

inline Vec3D Vec3D::unitVector() const
{
    return operator/(getLength());
}

inline Vec3D Vec3D::negate()
{
    return operator*(-1);
}

inline bool Vec3D::isIntEquivalent(Vec3D a)
{
    return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z);
}

// -----------------------   Vec4D    -----------------------

inline Vec3D Vec4D::getVector() const
{
    return Vec3D(x, y, z);
}

inline Vec4D Vec4D::homogenize() const
{
    return Vec4D(getVector() / w);
}

inline Vec4D Vec4D::unitVector()
{
    Vec4D homogenized = homogenize();
    return homogenized / homogenized.getVector().getLength();
}

inline float Vec4D::lengthSquared() const
{
    return homogenize().getVector().lengthSquared();
}

inline float Vec4D::getLength() const
{
    return sqrtf(lengthSquared());
}

inline Vec4D Vec4D::operator+(const Vec4D &a)
{
    return Vec4D(x + a.x, y + a.y, z + a.z, w + a.w);
}

inline Vec4D Vec4D::operator-(const Vec4D &a)
{
    return Vec4D(x - a.x, y - a.y, z - a.z, w - a.w);
}

inline Vec4D Vec4D::operator*(const Vec4D &a)
{
    return Vec4D(x * a.x, y * a.y, z * a.z, w * a.w);
}

inline Vec4D Vec4D::operator*(const float &a)
{
    return Vec4D(x * a, y * a, z * a, w * a);
}

inline Vec4D Vec4D::operator/(const float &b)
{
    if (b == 0.0f)
        return operator*(VEC_HIGH_VALUE);
    Vec4D res = operator*(1 / b);
    return res;
}

inline bool Vec4D::operator==(const Vec4D &b)
{
    return (x == b.x && y == b.y && z == b.z && w == b.w);
}

inline float& Vec4D::operator[](const size_t a)
{
    return *((&x)+a);
}

inline const float& Vec4D::operator[](const size_t a) const
{
   return *((&x)+a);
}

inline bool Vec4D::isCollinearWith(Vec4D a)
{
    float res = dotProduct(a);
    return (res < VEC_COLLINEAR_PRECISION);
}

inline float Vec4D::dotProduct(Vec4D a)
{
    return (a.x * x + a.y * y + a.z * z);
}

inline Vec4D Vec4D::crossProduct(Vec4D a)
{
    return Vec4D((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x), 1.0f);
}

inline Vec4D Vec4D::negate()
{
    return operator*(-1);
}

inline bool Vec4D::isIntEquivalent(Vec4D a)
{
    return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z && (int)w == (int)a.w);
}

// -----------------------   UChar4D    -----------------------

inline UChar4D::UChar4D(const float* in)
{
    r = (unsigned char)(in[0]*255);
    g = (unsigned char)(in[1]*255);
    b = (unsigned char)(in[2]*255);
    a = (unsigned char)(in[3]*255);
}

inline UChar4D::UChar4D(const Vec4D& in)
{
    r = (unsigned char)(math::cut(in[0],0,1)*255);
    g = (unsigned char)(math::cut(in[1],0,1)*255);
    b = (unsigned char)(math::cut(in[2],0,1)*255);
    a = (unsigned char)(math::cut(in[3],0,1)*255);
}

inline UChar4D UChar4D::operator*(const float &in)
{
    return UChar4D(r*in,g*in,b*in,a);
}

inline UChar4D UChar4D::operator+(const UChar4D &in)
{
    return UChar4D(r+in.r,g+in.g,b+in.b,a);
}

// -----------------------   Mat4D    -----------------------

inline float& Mat4D::operator[](const size_t in)
{
    return content[in / 4][in % 4];
}

inline float& Mat4D::at(const unsigned char x, const unsigned char y)
{
    if (x > 3 || y > 3)
        return content[0][0];
    return content[y][x];
}

// ----------------------- Math Utils -----------------------

inline float math::toRadians(float in)
{
    return in / 180.0 * M_PI;
}

inline float math::toDegrees(float in)
{
    return in * 180.0 / M_PI;
}

inline float math::cut(float in, float min, float max)
{
    if (in < min)
        in = min;
    if (in > max)
        in = max;
    return in;
}

inline float math::cutInt(int in, int min, int max)
{
    if (in < min)
        in = min;
    if (in > max)
        in = max;
    return in;
}

inline float math::mod(float in, float value)
{
    return in - value*floorf(in/value);
}

inline float math::min(float a, float b)
{
    if (a > b)
        return b;
    return a;
}

inline float math::max(float a, float b)
{
    if (a > b)
        return a;
    return b;
}

inline bool math::isEqual(float a, float b, float prec)
{
    return (a - prec < b && a + prec > b);
}

inline bool math::isEqualVec4(Vec4D a, Vec4D b, float prec)
{
    return (isEqual(b.x, a.x, prec) && isEqual(b.y, a.y, prec) && isEqual(b.z, a.z, prec) && isEqual(b.w, a.w, prec));
}

inline float math::edgeFunction(const Int2D p, const Int2D a, const Int2D b)
{
    return (p.x-a.x)*(b.y-a.y)-(p.y-a.y)*(b.x-a.x);
}

inline bool math::isTopLeft(const Int2D& a, const Int2D& b)
{
    return (a.y > b.y || (a.y == b.y && a.x < b.x));
}