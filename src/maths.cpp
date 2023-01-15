#include <cstdio>

#include "maths.hpp"
#include <Renderer.hpp>

// -----------------------   Vec3D    -----------------------

void Vec3D::print()
{
    printf("(%.2f, %.2f, %.2f)\n", x, y, z);
}

// -----------------------   Vec4D    -----------------------

void Vec4D::print()
{
    printf("(%.2f, %.2f, %.2f, %.2f)\n", x, y, z, w);
}

// -----------------------   Mat4D    -----------------------

Mat4D::Mat4D(float diagonal)
{
    for (size_t i = 0; i < 4; i++) content[i][i] = diagonal;
}

Mat4D::Mat4D(const Mat4D &in)
{
    for (size_t j = 0; j < 4; j++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            content[j][i] = in.content[j][i];
        }
    }
}

Mat4D::Mat4D(const float* data)
{
    for (size_t j = 0; j < 4; j++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            content[j][i] = data[j*4+i];
        }
    }
}

Mat4D Mat4D::operator*(const Mat4D &in)
{
    Mat4D out;
    for (size_t j = 0; j < 4; j++)
    {
        for (size_t i = 0; i < 4; i++)
        {
            float res = 0;
            for (size_t k = 0; k < 4; k++)
                res += content[j][k] * in.content[k][i];

            out.content[j][i] = res;
        }
    }
    return out;
}

Vec4D Mat4D::operator*(const Vec4D &in)
{
    Vec4D out;
    for (size_t i = 0; i < 4; i++)
    {
        float res = 0;
        for (size_t k = 0; k < 4; k++) res += content[i][k] * in[k];
        out[i] = res;
    }
    return out;
}

Mat4D Mat4D::CreateXRotationMatrix(float angle)
{
    Mat4D out = Mat4D(1);
    float radA = math::toRadians(angle);
    float cosA = cosf(radA);
    float sinA = sinf(radA);
    out.at(1,1) = cosA;
    out.at(2,1) = -sinA;
    out.at(1,2) = sinA;
    out.at(2,2) = cosA;
    return out;
}

Mat4D Mat4D::CreateYRotationMatrix(float angle)
{
    Mat4D out = Mat4D(1);
    float radA = math::toRadians(angle);
    float cosA = cosf(radA);
    float sinA = sinf(radA);
    out.at(0,0) = cosA;
    out.at(2,0) = sinA;
    out.at(0,2) = -sinA;
    out.at(2,2) = cosA;
    return out;
}
    
Mat4D Mat4D::CreateZRotationMatrix(float angle)
{
    Mat4D out = Mat4D(1);
    float radA = math::toRadians(angle);
    float cosA = cosf(radA);
    float sinA = sinf(radA);
    out.at(0,0) = cosA;
    out.at(1,0) = -sinA;
    out.at(0,1) = sinA;
    out.at(1,1) = cosA;
    return out;
}

Mat4D Mat4D::CreateScaleMatrix(const Vec3D& scale)
{
    Mat4D out;
    for (size_t i = 0; i < 3; i++) out.at(i,i) = scale[i];
    out.content[3][3] = 1;
    return out;
}

Mat4D Mat4D::CreateTranslationMatrix(const Vec3D& translation)
{
    Mat4D out = Mat4D(1);
    for (size_t i = 0; i < 3; i++) out.at(3,i) = translation[i];
    return out;
}

Mat4D Mat4D::CreateTransformMatrix(const Vec3D& position, const Vec3D& rotation, const Vec3D& scale)
{
    return CreateTranslationMatrix(position) * CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z) * CreateScaleMatrix(scale);
}

Mat4D Mat4D::CreatePerspectiveProjectionMatrix(float near, float far, float fov)
{
    float s = 1.0f/tanf(math::toRadians(fov/2.0f));
    float param1 = - far / (far - near);
    float param2 = param1*near;
    Mat4D out;
    out.at(0,0) = s;
    out.at(1,1) = s;
    out.at(2,2) = param1;
    out.at(3,2) = -1;
    out.at(2,3) = param2;
    return out;
}

Mat4D Mat4D::getCofactor(int p, int q, int n)
{
    Mat4D mat;
    int i = 0, j = 0;
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                mat.content[i][j++] = content[row][col];
 
                // Row is filled, so increase row index and
                // reset col index
                if (j == n-1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
    return mat;
}

int Mat4D::getDeterminant(int n)
{
    Mat4D a;
    int D = 0; // Initialize result
 
    //  Base case : if matrix contains single element
    if (n == 1)
        return content[0][0];
 
    int sign = 1;  // To store sign multiplier
 
     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of matrix[0][f]
        a = getCofactor(0, f, n);
        D += sign * content[0][f] * a.getDeterminant(n - 1);
 
        // terms are to be added with alternate sign
        sign = -sign;
    }
 
    return D;
}

Mat4D Mat4D::CreateInverseMatrix()
{
    // Find determinant of matrix
    Mat4D inverse;
    int det = getDeterminant(4);
    if (det == 0)
    {
        printf("Singular matrix, can't find its inverse\n");
        return 1;
    }
 
    // Find adjoint
    Mat4D adj = CreateAdjMatrix();
 
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            inverse.content[i][j] = adj.content[i][j]/float(det);
 
    return inverse;
}

Mat4D Mat4D::CreateAdjMatrix()
{ 
    // temp is used to store cofactors of matrix
    Mat4D temp;
    Mat4D adj;
    int sign = 1;
 
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            // Get cofactor of matrix[i][j]
            temp = getCofactor(i, j, 4);
 
            // sign of adj positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
 
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj.content[j][i] = (sign)*(temp.getDeterminant(3));
        }
    }
    return adj;
}


Mat4D Mat4D::transposeMatrix()
{
    float x[16] = {0};
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            x[i + j*4] = content[i][j];
        }
    }
    
    return Mat4D{x};
}

void Mat4D::printMatrix()
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (content[j][i] >= 0.0f) printf(" ");
            printf("%0.2f ",content[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

Mat4D Mat4D::Identity()
{
    return Mat4D(1);
}

// ----------------------- Math Utils -----------------------

void math::drawLine(UChar4D* data, const int& width, const int& msaa, Int2D a, Int2D b, const UChar4D& color)
{
    if (a.x > b.x)
    {
        Int2D temporary = a;
        a = b;
        b = temporary;
    }
    Int2D d;
    d.x = b.x - a.x;

    if (d.x != 0)
    {
        if (d.x > 0)
        {
            d.y = b.y - a.y;
            if (d.y != 0)
            {
                if (d.y > 0)
                {
                    // vecteur oblique dans le 1er cadran

                    if (d.x >= d.y)
                    {
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 1er octant
                        int e = d.x;
                        d.x = e * 2;
                        d.y = d.y * 2; // e est positif
                        while (true)
                        { // déplacements horizontaux
                            for (int i = 0; i < msaa*msaa; i++) data[(a.y * width + a.x)*msaa*msaa+i] = color;
                            a.x++;
                            if (a.x == b.x)
                                break;
                            e = e - d.y;
                            if (e < 0)
                            {
                                a.y++; // déplacement diagonal
                                e += d.x;
                            }
                        }
                    }
                    else
                    {
                        // vecteur oblique proche de la verticale, dans le 2d octant
                        int e = d.y;
                        d.y = e * 2;
                        d.x = d.x * 2; // e est positif
                        while (true)
                        { // déplacements verticaux
                            for (int i = 0; i < msaa*msaa; i++) data[(a.y * width + a.x)*msaa*msaa+i] = color;
                            a.y++;
                            if (a.y == b.y)
                                break;
                            e = e - d.x;
                            if (e < 0)
                            {
                                a.x++; // déplacement diagonal
                                e = e + d.y;
                            }
                        }
                    }
                }
                else // d.y < 0 (et d.x > 0)
                    // vecteur oblique dans le 4e cadran

                    if (d.x >= -d.y)
                {
                    // vecteur diagonal ou oblique proche de l’horizontale, dans le 8e octant
                    int e = d.x;
                    d.x = e * 2;
                    d.y = d.y * 2; // e est positif
                    while (true)
                    { // déplacements horizontaux
                        for (int i = 0; i < msaa*msaa; i++) data[(a.y * width + a.x)*msaa*msaa+i] = color;
                        a.x++;
                        if (a.x == b.x)
                            break;
                        e = e + d.y;
                        if (e < 0)
                        {
                            a.y--; // déplacement diagonal
                            e = e + d.x;
                        }
                    }
                }
                else // vecteur oblique proche de la verticale, dans le 7e octant
                {
                    int e = d.y;
                    d.y = e * 2;
                    d.x = d.x * 2; // e est négatif
                    while (true)
                    { // déplacements verticaux
                        for (int i = 0; i < msaa*msaa; i++) data[(a.y * width + a.x)*msaa*msaa+i] = color;
                        a.y--;
                        if (a.y == b.y)
                            break;
                        e = e + d.x;
                        if (e > 0)
                        {
                            a.x++; // déplacement diagonal
                            e = e + d.y;
                        }
                    }
                }
            }
            else // d.y = 0 (et d.x > 0)
            {
                // vecteur horizontal vers la droite
                do
                {
                    for (int i = 0; i < msaa*msaa; i++) data[(a.y * width + a.x)*msaa*msaa+i] = color;
                    a.x++;
                } while (a.x != b.x);
            }
        }
    }
    else
    { // d.x = 0
        d.y = b.y - a.y;
        if (d.y != 0)
        {
            if (d.y > 0)
            {

                // vecteur vertical croissant
                do
                {
                    for (int i = 0; i < msaa*msaa; i++) data[(a.y * width + a.x)*msaa*msaa+i] = color;
                    a.y++;
                } while (a.y != b.y);
            }
            else
            { // d.y < 0 (et d.x = 0)

                // vecteur vertical décroissant
                do
                {
                    for (int i = 0; i < msaa*msaa; i++) data[(a.y * width + a.x)*msaa*msaa+i] = color;
                    a.y--;
                } while (a.y != b.y);
            }
        }
    }
    // le pixel final (b.x, b.y) n’est pas tracé.
}