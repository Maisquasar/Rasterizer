#pragma once

#include <math.h>
#include <stdbool.h>
#include <filesystem>

#define VEC_COLLINEAR_PRECISION 0.001f
#define VEC_HIGH_VALUE 1e20f

class Vec2D;

class Int2D
{
    public:
    int x, y;
    inline Int2D() : x(0), y(0) {}
    inline Int2D(const Int2D &in) : x(in.x), y(in.y) {}
    inline Int2D(const Vec2D &in);
    inline Int2D(const int& a, const int& b) : x(a), y(b) {}

    inline Int2D operator+(const Int2D &a);

    // Return a new vector wich is the substraction of 'a' and 'b'
    inline Int2D operator-(const Int2D &a);

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Int2D operator*(const Int2D &a);

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Int2D operator*(const float &a);

    // Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
    inline Int2D operator/(const float &b);

    inline bool operator==(const Int2D &b);
};

class Vec2D
{
public:
    float x;
    float y;

    // Return a new empty Vec2D
    inline Vec2D() : x(0), y(0) {}

    // Return a new Vec2D initialised with 'a' and 'b'
    inline Vec2D(float a, float b) : x(a), y(b) {}

    // Return a new Vec2D initialised with 'in'
    inline Vec2D(const Vec2D &in) : x(in.x), y(in.y) {}
    inline Vec2D(const Int2D &in) : x(in.x), y(in.y) {}

    // Return the length squared of 'in'
    inline float lengthSquared();

    //Return the distance between 'in' an 'a'
    inline float getDistanceFromPoint(Vec2D a);

    // Return the lenght of the given Vector
    inline float getLength();

    // Return a new vector wich is the sum of 'a' and 'b'
    inline Vec2D operator+(const Vec2D &a);

    // Return a new vector wich is the substraction of 'a' and 'b'
    inline Vec2D operator-(const Vec2D &a);

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Vec2D operator*(const Vec2D &a);

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Vec2D operator*(const float &a);

    // Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
    inline Vec2D operator/(const float &b);

    inline bool operator==(const Vec2D &b);

    // Return tue if 'a' and 'b' are collinears (Precision defined by VEC2D_COLLINEAR_PRECISION)
    inline bool isCollinearWith(Vec2D a);

    // Return the dot product of 'a' and 'b'
    inline float dotProduct(Vec2D a);

    // Return the z component of the cross product of 'a' and 'b'
    inline float crossProduct(Vec2D a);

    // Return a vector with the same direction that 'in', but with length 1
    inline Vec2D unitVector();

    // Return a vector of length 'in' and with an opposite direction
    inline Vec2D negate();

    // Return the normal vector of 'in'
    inline Vec2D getNormal();

    // return true if 'a' converted to int is equivalent to 'in' converted to int
    inline bool isIntEquivalent(Vec2D a);

};

class Vec3D;

class Int3D
{
    public:
    int x, y, z;
    inline Int3D() : x(0), y(0), z(0) {}
    inline Int3D(const Int3D &in) : x(in.x), y(in.y), z(in.z) {}
    inline Int3D(const Vec3D &in);
    inline Int3D(const int& a, const int& b, const int& c) : x(a), y(b), z(c) {}

    inline Int3D operator+(const Int3D &a);

    // Return a new vector wich is the substraction of 'a' and 'b'
    inline Int3D operator-(const Int3D &a);

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Int3D operator*(const Int3D &a);

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Int3D operator*(const float &a);

    // Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
    inline Int3D operator/(const float &b);

    inline bool operator==(const Int3D &b);

    inline int& operator[](const size_t a);
};

class Vec3D
{
public:
    float x;
    float y;
    float z;

    // Return a new empty Vec3D
    inline Vec3D() : x(0), y(0), z(0) {}

    inline Vec3D(float content) : x(content), y(content), z(content) {}

    // Return a new Vec3D initialised with 'a' and 'b'
    inline Vec3D(float a, float b, float c) : x(a), y(b), z(c) {}

    // Return a new Vec3D initialised with 'in'
    inline Vec3D(const Vec3D &in) : x(in.x), y(in.y), z(in.z) {}

    // Print the Vec3D
    void print();

    // Return the length squared of 'in'
    inline float lengthSquared() const;

    // Return the lenght of the given Vector
    inline float getLength() const;

    // Return a new vector wich is the sum of 'a' and 'b'
    inline Vec3D operator+(const Vec3D &a) const;

    // Return a new vector wich is the substraction of 'a' and 'b'
    inline Vec3D operator-(const Vec3D &a) const;

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Vec3D operator*(const Vec3D &a) const;

    // Return the result of the aritmetic multiplication of 'a' and 'b'
    inline Vec3D operator*(const float &a) const;

    // Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC_HIGH_VALUE if 'b' is equal 0;
    inline Vec3D operator/(const float &b) const;

    inline bool operator==(const Vec3D &b) const;

    inline const float& operator[](const size_t a) const;

    inline float& operator[](const size_t a);

    // Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
    inline bool isCollinearWith(Vec3D a);

    // Return the dot product of 'a' and 'b'
    inline float dotProduct(Vec3D a);

    // Return the z component of the cross product of 'a' and 'b'
    inline Vec3D crossProduct(Vec3D a);

    // Return a vector with the same direction that 'in', but with length 1
    inline Vec3D unitVector() const;

    // Return a vector of length 'in' and with an opposite direction
    inline Vec3D negate();

    // return true if 'a' converted to int is equivalent to 'in' converted to int
    inline bool isIntEquivalent(Vec3D a);
};

class Vec4D;

class UChar4D
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    inline UChar4D() : r(0), g(0), b(0), a(0) {}
    inline UChar4D(const float* in);
    inline UChar4D(const Vec4D& in);
    inline UChar4D(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0xff) : r(red), g(green), b(blue), a(alpha) {}
    inline UChar4D(unsigned int rgba) : r((rgba & 0xff000000) >> 24), g((rgba & 0x00ff0000) >> 16), b((rgba & 0x0000ff00) >> 8), a(rgba & 0x000000ff) {}

    inline UChar4D operator*(const float &a);
    inline UChar4D operator+(const UChar4D &a);
};

class Vec4D
{
public:
    float x;
    float y;
    float z;
    float w;

    // Return a new empty Vec4D
    inline Vec4D() : x(0), y(0), z(0), w(0) {}

    // Return a new Vec4D initialised with 'a', 'b', 'c' and 'd'
    inline Vec4D(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}

    // Return a new Vec4D initialised with 'in'
    inline Vec4D(const Vec3D &in, float wIn = 1.0f) : x(in.x), y(in.y), z(in.z), w(wIn) {}

    // Return a new Vec4D initialised with 'in'
    inline Vec4D(const Vec4D &in) : x(in.x), y(in.y), z(in.z), w(in.w) {}

    inline Vec4D(const UChar4D &in) : x(in.r/255.0f), y(in.g/255.0f), z(in.b/255.0f), w(in.a/255.0f) {}


    // Print the Vec4D
    void print();

    // Return the Vec3D of Vec4D
    inline Vec3D getVector() const;

    // Return the length squared
    inline float lengthSquared() const;

    // Return the length
    inline float getLength() const;

    // Divide each components by w, or set to VEC_HIGH_VALUE if w equals 0
    inline Vec4D homogenize() const;

    inline Vec4D operator+(const Vec4D &a);

    inline Vec4D operator-(const Vec4D &a);

    inline Vec4D operator*(const Vec4D &a);

    inline Vec4D operator*(const float &a);

    inline Vec4D operator/(const float &b);

    inline bool operator==(const Vec4D &b);

    inline float& operator[](const size_t a);
    inline const float& operator[](const size_t a) const;

    // Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
    inline bool isCollinearWith(Vec4D a);

    inline float dotProduct(Vec4D a);

    // Return the z component of the cross product of 'a' and 'b'
    inline Vec4D crossProduct(Vec4D a);

    // Return a vector with the same direction that 'in', but with length 1
    inline Vec4D unitVector();

    // Return a vector of length 'in' and with an opposite direction
    inline Vec4D negate();

    // return true if 'a' converted to int is equivalent to 'in' converted to int
    inline bool isIntEquivalent(Vec4D a);
};

class Mat4D
{
public:
    /* data of the matrix : content[y][x]
     * Matrix is indexed with:
     * 
     * 00 | 01 | 02 | 03
     * 10 | 11 | 12 | 13
     * 20 | 21 | 22 | 23
     * 30 | 31 | 32 | 33
     * 
    */
    float content[4][4] = {0};

    // Return a new empty Vec4D
    Mat4D() {}

    Mat4D(float diagonal);

    Mat4D(const Mat4D& in);

    Mat4D(const float* data);

    Mat4D operator*(const Mat4D &a);

    Vec4D operator*(const Vec4D &a);

    static Mat4D Identity();

    static Mat4D CreateTransformMatrix(const Vec3D& position, const Vec3D& rotation, const Vec3D& scale);

    static Mat4D CreateTranslationMatrix(const Vec3D& translation);

    static Mat4D CreateScaleMatrix(const Vec3D& scale);

    static Mat4D CreateXRotationMatrix(float angle);

    static Mat4D CreateYRotationMatrix(float angle);
    
    static Mat4D CreateZRotationMatrix(float angle);

    static Mat4D CreatePerspectiveProjectionMatrix(float near, float far, float fov);

    Mat4D CreateInverseMatrix();

    Mat4D CreateAdjMatrix();

    Mat4D getCofactor(int p, int q, int n);

    // Recursive function for finding determinant of matrix. n is current dimension of 'in'.
    int getDeterminant(int n);

    Mat4D transposeMatrix();

    inline float& operator[](const size_t a);

    inline float& at(const unsigned char x, const unsigned char y);

    void printMatrix();

};

struct Viewport;

namespace math
{
    // Return the given angular value in degrees converted to radians
    inline float toRadians(float in);

    // Return the given angular value in radians converted to degrees
    inline float toDegrees(float in);

    inline float cut(float in, float min, float max);

    inline float cutInt(int in, int min, int max);

    inline float mod(float in, float value);

    inline float min(float a, float b);

    inline float max(float a, float b);

    inline bool isEqual(float a, float b, float prec);

    inline bool isEqualVec4(Vec4D a, Vec4D b, float prec);

    // Draw lines with given data.
    //Source: https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham
    void drawLine(UChar4D* data, const int& width, const int& msaa, Int2D a, Int2D b, const UChar4D& color);

    inline float edgeFunction(const Int2D p, const Int2D a, const Int2D b);

    inline bool isTopLeft(const Int2D& a, const Int2D& b);
};

#include "maths.inl"