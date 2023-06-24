// Math3d.h
// Header file for the Math3d library. The C-Runtime has math.h, this file and the
// accompanying math.c are meant to suppliment math.h by adding geometry/math routines
// useful for graphics, simulation, and physics applications (3D stuff).
// Richard S. Wright Jr.
#ifndef _MATH3D_LIBRARY__
#define _MATH3D_LIBRARY__

#include <math.h>
#include <memory.h>

///////////////////////////////////////////////////////////////////////////////
// Data structures and containers
// Much thought went into how these are declared. Many libraries declare these
// as structures with x, y, z data members. However structure alignment issues
// could limit the portability of code based on such structures, or the binary
// compatibility of data files (more likely) that contain such structures across
// compilers/platforms. Arrays are always tightly packed, and are more efficient 
// for moving blocks of data around (usually).
using M3DVector3f = float[3]; // Vector of three floats (x, y, z)
using M3DVector3d = double[3]; // Vector of three doubles (x, y, z)

using M3DVector4f = float[4]; // Lesser used... Do we really need these?
using M3DVector4d = double[4]; // Yes, occasionaly

using M3DVector2f = float[2]; // 3D points = 3D Vectors, but we need a 
using M3DVector2d = double[2]; // 2D representations sometimes... (x,y) order


// 3x3 matrix - column major. X vector is 0, 1, 2, etc.
//		0	3	6	
//		1	4	7
//		2	5	8
using M3DMatrix33f = float[9]; // A 3 x 3 matrix, column major (floats) - OpenGL Style
using M3DMatrix33d = double[9]; // A 3 x 3 matrix, column major (doubles) - OpenGL Style


// 4x4 matrix - column major. X vector is 0, 1, 2, etc.
//	0	4	8	12
//	1	5	9	13
//	2	6	10	14
//	3	7	11	15
using M3DMatrix44f = float[16]; // A 4 X 4 matrix, column major (floats) - OpenGL style
using M3DMatrix44d = double[16]; // A 4 x 4 matrix, column major (doubles) - OpenGL style


///////////////////////////////////////////////////////////////////////////////
// Useful constants
#define M3D_PI (3.14159265358979323846)
#define M3D_2PI (2.0 * M3D_PI)
#define M3D_PI_DIV_180 (0.017453292519943296)
#define M3D_INV_PI_DIV_180 (57.2957795130823229)


///////////////////////////////////////////////////////////////////////////////
// Useful shortcuts and macros
// Radians are king... but we need a way to swap back and forth
#define m3dDegToRad(x)	((x)*M3D_PI_DIV_180)
#define m3dRadToDeg(x)	((x)*M3D_INV_PI_DIV_180)

// Hour angles
#define m3dHrToDeg(x)	((x) * (1.0 / 15.0))
#define m3dHrToRad(x)	m3dDegToRad(m3dHrToDeg(x))

#define m3dDegToHr(x)	((x) * 15.0))
#define m3dRadToHr(x)	m3dDegToHr(m3dRadToDeg(x))


// Returns the same number if it is a power of
// two. Returns a larger integer if it is not a 
// power of two. The larger integer is the next
// highest power of two.
inline unsigned int m3dIsPOW2(unsigned int iValue)
{
    unsigned int nPow2 = 1;

    while (iValue > nPow2)
        nPow2 = (nPow2 << 1);

    return nPow2;
}


///////////////////////////////////////////////////////////////////////////////
// Inline accessor functions for people who just can't count to 3 - Vectors
#define	m3dGetVectorX(v) (v[0])
#define m3dGetVectorY(v) (v[1])
#define m3dGetVectorZ(v) (v[2])
#define m3dGetVectorW(v) (v[3])

#define m3dSetVectorX(v, x)	((v)[0] = (x))
#define m3dSetVectorY(v, y)	((v)[1] = (y))
#define m3dSetVectorZ(v, z)	((v)[2] = (z))
#define m3dSetVectorW(v, w)	((v)[3] = (w))

///////////////////////////////////////////////////////////////////////////////
// Inline vector functions
// Load Vector with (x, y, z, w).
inline void m3dLoadVector2(M3DVector2f v, float x, float y)
{
    v[0] = x;
    v[1] = y;
}

inline void m3dLoadVector2(M3DVector2d v, float x, float y)
{
    v[0] = x;
    v[1] = y;
}

inline void m3dLoadVector3(M3DVector3f v, float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

inline void m3dLoadVector3(M3DVector3d v, double x, double y, double z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

inline void m3dLoadVector4(M3DVector4f v, float x, float y, float z, float w)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

inline void m3dLoadVector4(M3DVector4d v, double x, double y, double z, double w)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}


////////////////////////////////////////////////////////////////////////////////
// Copy vector src into vector dst
inline void m3dCopyVector2(M3DVector2f dst, const M3DVector2f src) { memcpy(dst, src, sizeof(M3DVector2f)); }
inline void m3dCopyVector2(M3DVector2d dst, const M3DVector2d src) { memcpy(dst, src, sizeof(M3DVector2d)); }

inline void m3dCopyVector3(M3DVector3f dst, const M3DVector3f src) { memcpy(dst, src, sizeof(M3DVector3f)); }
inline void m3dCopyVector3(M3DVector3d dst, const M3DVector3d src) { memcpy(dst, src, sizeof(M3DVector3d)); }

inline void m3dCopyVector4(M3DVector4f dst, const M3DVector4f src) { memcpy(dst, src, sizeof(M3DVector4f)); }
inline void m3dCopyVector4(M3DVector4d dst, const M3DVector4d src) { memcpy(dst, src, sizeof(M3DVector4d)); }


////////////////////////////////////////////////////////////////////////////////
// Add Vectors (r, a, b) r = a + b
inline void m3dAddVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
}

inline void m3dAddVectors2(M3DVector2d r, const M3DVector2d a, const M3DVector2d b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
}

inline void m3dAddVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
}

inline void m3dAddVectors3(M3DVector3d r, const M3DVector3d a, const M3DVector3d b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
}

inline void m3dAddVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
    r[3] = a[3] + b[3];
}

inline void m3dAddVectors4(M3DVector4d r, const M3DVector4d a, const M3DVector4d b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
    r[3] = a[3] + b[3];
}

////////////////////////////////////////////////////////////////////////////////
// Subtract Vectors (r, a, b) r = a - b
inline void m3dSubtractVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
}

inline void m3dSubtractVectors2(M3DVector2d r, const M3DVector2d a, const M3DVector2d b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
}

inline void m3dSubtractVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

inline void m3dSubtractVectors3(M3DVector3d r, const M3DVector3d a, const M3DVector3d b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

inline void m3dSubtractVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];
}

inline void m3dSubtractVectors4(M3DVector4d r, const M3DVector4d a, const M3DVector4d b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];
}


///////////////////////////////////////////////////////////////////////////////////////
// Scale Vectors (in place)
inline void m3dScaleVector2(M3DVector2f v, float scale)
{
    v[0] *= scale;
    v[1] *= scale;
}

inline void m3dScaleVector2(M3DVector2d v, double scale)
{
    v[0] *= scale;
    v[1] *= scale;
}

inline void m3dScaleVector3(M3DVector3f v, float scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

inline void m3dScaleVector3(M3DVector3d v, double scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

inline void m3dScaleVector4(M3DVector4f v, float scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
    v[3] *= scale;
}

inline void m3dScaleVector4(M3DVector4d v, double scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
    v[3] *= scale;
}


//////////////////////////////////////////////////////////////////////////////////////
// Cross Product
// u x v = result
// We only need one version for floats, and one version for doubles. A 3 component
// vector fits in a 4 component vector. If  M3DVector4d or M3DVector4f are passed
// we will be OK because 4th component is not used.
inline void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v)
{
    result[0] = u[1] * v[2] - v[1] * u[2];
    result[1] = -u[0] * v[2] + v[0] * u[2];
    result[2] = u[0] * v[1] - v[0] * u[1];
}

inline void m3dCrossProduct(M3DVector3d result, const M3DVector3d u, const M3DVector3d v)
{
    result[0] = u[1] * v[2] - v[1] * u[2];
    result[1] = -u[0] * v[2] + v[0] * u[2];
    result[2] = u[0] * v[1] - v[0] * u[1];
}

//////////////////////////////////////////////////////////////////////////////////////
// Dot Product, only for three component vectors
// return u dot v
inline float m3dDotProduct(const M3DVector3f u, const M3DVector3f v)
{
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline double m3dDotProduct(const M3DVector3d u, const M3DVector3d v)
{
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

//////////////////////////////////////////////////////////////////////////////////////
// Angle between vectors, only for three component vectors. Angle is in radians...
inline float m3dGetAngleBetweenVectors(const M3DVector3f u, const M3DVector3f v)
{
    float dTemp = m3dDotProduct(u, v);
    return static_cast<float>(acos(double(dTemp)));
}

inline double m3dGetAngleBetweenVectors(const M3DVector3d u, const M3DVector3d v)
{
    double dTemp = m3dDotProduct(u, v);
    return acos(dTemp);
}

//////////////////////////////////////////////////////////////////////////////////////
// Get Square of a vectors length
// Only for three component vectors
inline float m3dGetVectorLengthSquared(const M3DVector3f u)
{
    return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
}

inline double m3dGetVectorLengthSquared(const M3DVector3d u)
{
    return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
}

//////////////////////////////////////////////////////////////////////////////////////
// Get lenght of vector
// Only for three component vectors.
inline float m3dGetVectorLength(const M3DVector3f u)
{
    return static_cast<float>(sqrt(double(m3dGetVectorLengthSquared(u))));
}

inline double m3dGetVectorLength(const M3DVector3d u)
{
    return sqrt(m3dGetVectorLengthSquared(u));
}

//////////////////////////////////////////////////////////////////////////////////////
// Normalize a vector
// Scale a vector to unit length. Easy, just scale the vector by it's length
inline void m3dNormalizeVector(M3DVector3f u)
{
    m3dScaleVector3(u, 1.0f / m3dGetVectorLength(u));
}

inline void m3dNormalizeVector(M3DVector3d u)
{
    m3dScaleVector3(u, 1.0 / m3dGetVectorLength(u));
}


//////////////////////////////////////////////////////////////////////////////////////
// Get the distance between two points. The distance between two points is just
// the magnitude of the difference between two vectors
// Located in math.cpp
float m3dGetDistanceSquared(const M3DVector3f u, const M3DVector3f v)
{
	M3DVector3f d;
	d[0] = u[0] - v[0];
	d[1] = u[1] - v[1];
	d[2] = u[2] - v[2];
	return m3dGetVectorLengthSquared(d);
}
double m3dGetDistanceSquared(const M3DVector3d u, const M3DVector3d v)
{
	M3DVector3d d;
	d[0] = u[0] - v[0];
	d[1] = u[1] - v[1];
	d[2] = u[2] - v[2];
	return m3dGetVectorLengthSquared(d);
}

inline double m3dGetDistance(const M3DVector3d u, const M3DVector3d v)
{
    return sqrt(m3dGetDistanceSquared(u, v));
}

inline float m3dGetDistance(const M3DVector3f u, const M3DVector3f v)
{
    return static_cast<float>(sqrt(m3dGetDistanceSquared(u, v)));
}

inline float m3dGetMagnitudeSquared(const M3DVector3f u) { return u[0] * u[0] + u[1] * u[1] + u[2] * u[2]; }
inline double m3dGetMagnitudeSquared(const M3DVector3d u) { return u[0] * u[0] + u[1] * u[1] + u[2] * u[2]; }

inline float m3dGetMagnitude(const M3DVector3f u) { return static_cast<float>(sqrt(m3dGetMagnitudeSquared(u))); }
inline double m3dGetMagnitude(const M3DVector3d u) { return sqrt(m3dGetMagnitudeSquared(u)); }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Matrix functions
// Both floating point and double precision 3x3 and 4x4 matricies are supported.
// No support is included for arbitrarily dimensioned matricies on purpose, since
// the 3x3 and 4x4 matrix routines are the most common for the purposes of this
// library. Matrices are column major, like OpenGL matrices.
// Unlike the vector functions, some of these are going to have to not be inlined,
// although many will be.


// Copy Matrix
// Brain-dead memcpy
inline void m3dCopyMatrix33(M3DMatrix33f dst, const M3DMatrix33f src)
{
    memcpy(dst, src, sizeof(M3DMatrix33f));
}

inline void m3dCopyMatrix33(M3DMatrix33d dst, const M3DMatrix33d src)
{
    memcpy(dst, src, sizeof(M3DMatrix33d));
}

inline void m3dCopyMatrix44(M3DMatrix44f dst, const M3DMatrix44f src)
{
    memcpy(dst, src, sizeof(M3DMatrix44f));
}

inline void m3dCopyMatrix44(M3DMatrix44d dst, const M3DMatrix44d src)
{
    memcpy(dst, src, sizeof(M3DMatrix44d));
}

// LoadIdentity
// Implemented in Math3d.cpp
void m3dLoadIdentity33(M3DMatrix33f m);
void m3dLoadIdentity33(M3DMatrix33d m);
void m3dLoadIdentity44(M3DMatrix44f m);
void m3dLoadIdentity44(M3DMatrix44d m);

/////////////////////////////////////////////////////////////////////////////
// Get/Set Column.
inline void m3dGetMatrixColumn33(M3DVector3f dst, const M3DMatrix33f src, int column)
{
    memcpy(dst, src + (3 * column), sizeof(float) * 3);
}

inline void m3dGetMatrixColumn33(M3DVector3d dst, const M3DMatrix33d src, int column)
{
    memcpy(dst, src + (3 * column), sizeof(double) * 3);
}

inline void m3dSetMatrixColumn33(M3DMatrix33f dst, const M3DVector3f src, int column)
{
    memcpy(dst + (3 * column), src, sizeof(float) * 3);
}

inline void m3dSetMatrixColumn33(M3DMatrix33d dst, const M3DVector3d src, int column)
{
    memcpy(dst + (3 * column), src, sizeof(double) * 3);
}

inline void m3dGetMatrixColumn44(M3DVector4f dst, const M3DMatrix44f src, int column)
{
    memcpy(dst, src + (4 * column), sizeof(float) * 4);
}

inline void m3dGetMatrixColumn44(M3DVector4d dst, const M3DMatrix44d src, int column)
{
    memcpy(dst, src + (4 * column), sizeof(double) * 4);
}

inline void m3dSetMatrixColumn44(M3DMatrix44f dst, const M3DVector4f src, int column)
{
    memcpy(dst + (4 * column), src, sizeof(float) * 4);
}

inline void m3dSetMatrixColumn44(M3DMatrix44d dst, const M3DVector4d src, int column)
{
    memcpy(dst + (4 * column), src, sizeof(double) * 4);
}


// Get/Set row purposely omitted... use the functions below.
// I don't think row vectors are useful for column major ordering...
// If I'm wrong, add them later.


//////////////////////////////////////////////////////////////////////////////
// Get/Set RowCol - Remember column major ordering...
// Provides for element addressing
inline void m3dSetMatrixRowCol33(M3DMatrix33f m, int row, int col, float value)
{
    m[(col * 3) + row] = value;
}

inline float m3dGetMatrixRowCol33(const M3DMatrix33f m, int row, int col)
{
    return m[(col * 3) + row];
}

inline void m3dSetMatrixRowCol33(M3DMatrix33d m, int row, int col, double value)
{
    m[(col * 3) + row] = value;
}

inline double m3dGetMatrixRowCol33(const M3DMatrix33d m, int row, int col)
{
    return m[(col * 3) + row];
}

inline void m3dSetMatrixRowCol44(M3DMatrix44f m, int row, int col, float value)
{
    m[(col * 4) + row] = value;
}

inline float m3dGetMatrixRowCol44(const M3DMatrix44f m, int row, int col)
{
    return m[(col * 4) + row];
}

inline void m3dSetMatrixRowCol44(M3DMatrix44d m, int row, int col, double value)
{
    m[(col * 4) + row] = value;
}

inline double m3dGetMatrixRowCol44(const M3DMatrix44d m, int row, int col)
{
    return m[(col * 4) + row];
}


///////////////////////////////////////////////////////////////////////////////
// Extract a rotation matrix from a 4x4 matrix
// Extracts the rotation matrix (3x3) from a 4x4 matrix
inline void m3dExtractRotation(M3DMatrix33f dst, const M3DMatrix44f src)
{
    memcpy(dst, src, sizeof(float) * 3); // X column
    memcpy(dst + 3, src + 4, sizeof(float) * 3); // Y column
    memcpy(dst + 6, src + 8, sizeof(float) * 3); // Z column
}

// Ditto above, but for doubles
inline void m3dExtractRotation(M3DMatrix33d dst, const M3DMatrix44d src)
{
    memcpy(dst, src, sizeof(double) * 3); // X column
    memcpy(dst + 3, src + 4, sizeof(double) * 3); // Y column
    memcpy(dst + 6, src + 8, sizeof(double) * 3); // Z column
}

// Inject Rotation (3x3) into a full 4x4 matrix...
inline void m3dInjectRotation(M3DMatrix44f dst, const M3DMatrix33f src)
{
    memcpy(dst, src, sizeof(float) * 4);
    memcpy(dst + 4, src + 4, sizeof(float) * 4);
    memcpy(dst + 8, src + 8, sizeof(float) * 4);
}

// Ditto above for doubles
inline void m3dInjectRotation(M3DMatrix44d dst, const M3DMatrix33d src)
{
    memcpy(dst, src, sizeof(double) * 4);
    memcpy(dst + 4, src + 4, sizeof(double) * 4);
    memcpy(dst + 8, src + 8, sizeof(double) * 4);
}


////////////////////////////////////////////////////////////////////////////////
// MultMatrix
// Implemented in Math.cpp
void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b)
{
	product[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	product[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
	product[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
	product[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];

	product[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	product[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
	product[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
	product[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];

	product[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	product[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	product[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
	product[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
}
void m3dMatrixMultiply44(M3DMatrix44d product, const M3DMatrix44d a, const M3DMatrix44d b)
{
    product[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
    product[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
    product[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
    product[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];

    product[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
    product[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
    product[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
    product[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];

    product[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
    product[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
    product[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
    product[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
}
void m3dMatrixMultiply33(M3DMatrix33f product, const M3DMatrix33f a, const M3DMatrix33f b)
{
	product[0] = a[0] * b[0] + a[3] * b[1] + a[6] * b[2];
	product[3] = a[0] * b[3] + a[3] * b[4] + a[6] * b[5];
	product[6] = a[0] * b[6] + a[3] * b[7] + a[6] * b[8];

	product[1] = a[1] * b[0] + a[4] * b[1] + a[7] * b[2];
	product[4] = a[1] * b[3] + a[4] * b[4] + a[7] * b[5];
	product[7] = a[1] * b[6] + a[4] * b[7] + a[7] * b[8];

	product[2] = a[2] * b[0] + a[5] * b[1] + a[8] * b[2];
	product[5] = a[2] * b[3] + a[5] * b[4] + a[8] * b[5];
	product[8] = a[2] * b[6] + a[5] * b[7] + a[8] * b[8];
}
void m3dMatrixMultiply33(M3DMatrix33d product, const M3DMatrix33d a, const M3DMatrix33d b)
{
    product[0] = a[0] * b[0] + a[3] * b[1] + a[6] * b[2];
    product[3] = a[0] * b[3] + a[3] * b[4] + a[6] * b[5];
    product[6] = a[0] * b[6] + a[3] * b[7] + a[6] * b[8];

    product[1] = a[1] * b[0] + a[4] * b[1] + a[7] * b[2];
    product[4] = a[1] * b[3] + a[4] * b[4] + a[7] * b[5];
    product[7] = a[1] * b[6] + a[4] * b[7] + a[7] * b[8];

    product[2] = a[2] * b[0] + a[5] * b[1] + a[8] * b[2];
    product[5] = a[2] * b[3] + a[5] * b[4] + a[8] * b[5];
    product[8] = a[2] * b[6] + a[5] * b[7] + a[8] * b[8];
}


// Transform - Does rotation and translation via a 4x4 matrix. Transforms
// a point or vector.
// By-the-way __inline means I'm asking the compiler to do a cost/benefit analysis. If 
// these are used frequently, they may not be inlined to save memory. I'm experimenting
// with this....
__inline void m3dTransformVector3(M3DVector3f vOut, const M3DVector3f v, const M3DMatrix44f m)
{
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12]; // * v[3];	 
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13]; // * v[3];	
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14]; // * v[3];	
    //vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}

// Ditto above, but for doubles
__inline void m3dTransformVector3(M3DVector3d vOut, const M3DVector3d v, const M3DMatrix44d m)
{
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12]; // * v[3];	 
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13]; // * v[3];	
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14]; // * v[3];	
    //vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}

__inline void m3dTransformVector4(M3DVector4f vOut, const M3DVector4f v, const M3DMatrix44f m)
{
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
    vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}

// Ditto above, but for doubles
__inline void m3dTransformVector4(M3DVector4d vOut, const M3DVector4d v, const M3DMatrix44d m)
{
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
    vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}


// Just do the rotation, not the translation... this is usually done with a 3x3
// Matrix.
__inline void m3dRotateVector(M3DVector3f vOut, const M3DVector3f p, const M3DMatrix33f m)
{
    vOut[0] = m[0] * p[0] + m[3] * p[1] + m[6] * p[2];
    vOut[1] = m[1] * p[0] + m[4] * p[1] + m[7] * p[2];
    vOut[2] = m[2] * p[0] + m[5] * p[1] + m[8] * p[2];
}

// Ditto above, but for doubles
__inline void m3dRotateVector(M3DVector3d vOut, const M3DVector3d p, const M3DMatrix33d m)
{
    vOut[0] = m[0] * p[0] + m[3] * p[1] + m[6] * p[2];
    vOut[1] = m[1] * p[0] + m[4] * p[1] + m[7] * p[2];
    vOut[2] = m[2] * p[0] + m[5] * p[1] + m[8] * p[2];
}


// Scale a matrix (I don't beleive in Scaling matricies ;-)
// Yes, it's faster to loop backwards... These could be 
// unrolled... but eh... if you find this is a bottleneck,
// then you should unroll it yourself
inline void m3dScaleMatrix33(M3DMatrix33f m, float scale)
{
    for (int i = 8; i >= 0; i--) m[i] *= scale;
}

inline void m3dScaleMatrix33(M3DMatrix33d m, double scale)
{
    for (int i = 8; i >= 0; i--) m[i] *= scale;
}

inline void m3dScaleMatrix44(M3DMatrix44f m, float scale)
{
    for (int i = 15; i >= 0; i--) m[i] *= scale;
}

inline void m3dScaleMatrix44(M3DMatrix44d m, double scale)
{
    for (int i = 15; i >= 0; i--) m[i] *= scale;
}


// Create a Rotation matrix
// Implemented in math.cpp
void m3dRotationMatrix33(M3DMatrix33f m, float angle, float x, float y, float z)
{
    // Convert to radians
    angle = float((angle / 180.0f) * 3.14159265358979323846f);

    // Calculate sin and cosine of angle
    float sinAngle = sin(angle);
    float cosAngle = cos(angle);

    // Calculate 1 - cos and xx, xy, xz, yy, yz, zz values
    float oneMinusCos = 1.0f - cosAngle;
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float yy = y * y;
    float yz = y * z;
    float zz = z * z;

    // Set matrix values
    m[0] = xx + (cosAngle * (1.0f - xx));
    m[1] = (xy - (cosAngle * xy)) + (sinAngle * z);
    m[2] = (xz - (cosAngle * xz)) - (sinAngle * y);
    m[3] = (xy - (cosAngle * xy)) - (sinAngle * z);
    m[4] = yy + (cosAngle * (1.0f - yy));
    m[5] = (yz - (cosAngle * yz)) + (sinAngle * x);
    m[6] = (xz - (cosAngle * xz)) + (sinAngle * y);
    m[7] = (yz - (cosAngle * yz)) - (sinAngle * x);
    m[8] = zz + (cosAngle * (1.0f - zz));
}
void m3dRotationMatrix33(M3DMatrix33d m, double angle, double x, double y, double z)
{
    // Convert to radians
    angle = double((angle / 180.0f) * 3.14159265358979323846f);

    // Calculate sin and cosine of angle
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);

    // Calculate 1 - cos and xx, xy, xz, yy, yz, zz values
    double oneMinusCos = 1.0f - cosAngle;
    double xx = x * x;
    double xy = x * y;
    double xz = x * z;
    double yy = y * y;
    double yz = y * z;
    double zz = z * z;

    // Set matrix values
    m[0] = xx + (cosAngle * (1.0f - xx));
    m[1] = (xy - (cosAngle * xy)) + (sinAngle * z);
    m[2] = (xz - (cosAngle * xz)) - (sinAngle * y);
    m[3] = (xy - (cosAngle * xy)) - (sinAngle * z);
    m[4] = yy + (cosAngle * (1.0f - yy));
    m[5] = (yz - (cosAngle * yz)) + (sinAngle * x);
    m[6] = (xz - (cosAngle * xz)) + (sinAngle * y);
    m[7] = (yz - (cosAngle * yz)) - (sinAngle * x);
    m[8] = zz + (cosAngle * (1.0f - zz));
}
void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z)
{
    // Convert to radians
    angle = float((angle / 180.0f) * 3.14159265358979323846f);

    // Calculate sin and cosine of angle
    float sinAngle = sin(angle);
    float cosAngle = cos(angle);

    // Calculate 1 - cos and xx, xy, xz, yy, yz, zz values
    float oneMinusCos = 1.0f - cosAngle;
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float yy = y * y;
    float yz = y * z;
    float zz = z * z;

    // Set matrix values
    m[0] = xx + (cosAngle * (1.0f - xx));
    m[1] = (xy - (cosAngle * xy)) + (sinAngle * z);
    m[2] = (xz - (cosAngle * xz)) - (sinAngle * y);
    m[3] = 0.0f;

    m[4] = (xy - (cosAngle * xy)) - (sinAngle * z);
    m[5] = yy + (cosAngle * (1.0f - yy));
    m[6] = (yz - (cosAngle * yz)) + (sinAngle * x);
    m[7] = 0.0f;

    m[8] = (xz - (cosAngle * xz)) + (sinAngle * y);
    m[9] = (yz - (cosAngle * yz)) - (sinAngle * x);
    m[10] = zz + (cosAngle * (1.0f - zz));
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}
void m3dRotationMatrix44(M3DMatrix44d m, double angle, double x, double y, double z)
{
    // Convert to radians
    angle = double((angle / 180.0f) * 3.14159265358979323846f);

    // Calculate sin and cosine of angle
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);

    // Calculate 1 - cos and xx, xy, xz, yy, yz, zz values
    double oneMinusCos = 1.0f - cosAngle;
    double xx = x * x;
    double xy = x * y;
    double xz = x * z;
    double yy = y * y;
    double yz = y * z;
    double zz = z * z;

    // Set matrix values
    m[0] = xx + (cosAngle * (1.0f - xx));
    m[1] = (xy - (cosAngle * xy)) + (sinAngle * z);
    m[2] = (xz - (cosAngle * xz)) - (sinAngle * y);
    m[3] = 0.0f;

    m[4] = (xy - (cosAngle * xy)) - (sinAngle * z);
    m[5] = yy + (cosAngle * (1.0f - yy));
    m[6] = (yz - (cosAngle * yz)) + (sinAngle * x);
    m[7] = 0.0f;

    m[8] = (xz - (cosAngle * xz)) + (sinAngle * y);
    m[9] = (yz - (cosAngle * yz)) - (sinAngle * x);
    m[10] = zz + (cosAngle * (1.0f - zz));
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

// Create a Translation matrix. Only 4x4 matrices have translation components
inline void m3dTranslationMatrix44(M3DMatrix44f m, float x, float y, float z)
{
    m3dLoadIdentity44(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

inline void m3dTranslationMatrix44(M3DMatrix44d m, double x, double y, double z)
{
    m3dLoadIdentity44(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}


// Translate matrix. Only 4x4 matrices supported
inline void m3dTranslateMatrix44(M3DMatrix44f m, float x, float y, float z)
{
    m[12] += x;
    m[13] += y;
    m[14] += z;
}

inline void m3dTranslateMatrix44(M3DMatrix44d m, double x, double y, double z)
{
    m[12] += x;
    m[13] += y;
    m[14] += z;
}


// Scale matrix. Only 4x4 matrices supported
inline void m3dScaleMatrix44(M3DMatrix44f m, float x, float y, float z)
{
    m[0] *= x;
    m[5] *= y;
    m[10] *= z;
}

inline void m3dScaleMatrix44(M3DMatrix44d m, double x, double y, double z)
{
    m[0] *= x;
    m[5] *= y;
    m[10] *= z;
}


// Transpose/Invert - Only 4x4 matricies supported
#define TRANSPOSE44(dst, src)            \
{                                        \
    for (int j = 0; j < 4; j++)          \
    {                                    \
        for (int i = 0; i < 4; i++)      \
        {                                \
            dst[(j*4)+i] = src[(i*4)+j]; \
        }                                \
    }                                    \
}

inline void m3dTransposeMatrix44(M3DMatrix44f dst, const M3DMatrix44f src)
{
    TRANSPOSE44(dst, src);
}

inline void m3dTransposeMatrix44(M3DMatrix44d dst, const M3DMatrix44d src)
{
    TRANSPOSE44(dst, src);
}

bool m3dInvertMatrix44(M3DMatrix44f dst, const M3DMatrix44f src);
bool m3dInvertMatrix44(M3DMatrix44d dst, const M3DMatrix44d src);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Other Miscellaneous functions

// Find a normal from three points
// Implemented in math3d.cpp
void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2,
                   const M3DVector3f point3)
{
    M3DVector3f v1, v2; // Temporary vectors

    // Calculate two vectors from the three points. Assumes counter clockwise
    // winding!
    v1[0] = point1[0] - point2[0];
    v1[1] = point1[1] - point2[1];
    v1[2] = point1[2] - point2[2];

    v2[0] = point2[0] - point3[0];
    v2[1] = point2[1] - point3[1];
    v2[2] = point2[2] - point3[2];

    // Take the cross product of the two vectors to get
    // the normal vector.
    m3dCrossProduct(result, v1, v2);
}
void m3dFindNormal(M3DVector3d result, const M3DVector3d point1, const M3DVector3d point2,
                   const M3DVector3d point3)
{
    M3DVector3d v1, v2; // Temporary vectors

    // Calculate two vectors from the three points. Assumes counter clockwise
    // winding!
    v1[0] = point1[0] - point2[0];
    v1[1] = point1[1] - point2[1];
    v1[2] = point1[2] - point2[2];

    v2[0] = point2[0] - point3[0];
    v2[1] = point2[1] - point3[1];
    v2[2] = point2[2] - point3[2];

    // Take the cross product of the two vectors to get
    // the normal vector.
    m3dCrossProduct(result, v1, v2);
}


// Calculates the signed distance of a point to a plane
inline float m3dGetDistanceToPlane(const M3DVector3f point, const M3DVector4f plane)
{
    return point[0] * plane[0] + point[1] * plane[1] + point[2] * plane[2] + plane[3];
}

inline double m3dGetDistanceToPlane(const M3DVector3d point, const M3DVector4d plane)
{
    return point[0] * plane[0] + point[1] * plane[1] + point[2] * plane[2] + plane[3];
}


// Get plane equation from three points and a normal
void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3)
{
    // Get two vectors do the cross product
    M3DVector3f v1, v2;

    // V1 = p3 - p1;
    v1[0] = p3[0] - p1[0];
    v1[1] = p3[1] - p1[1];
    v1[2] = p3[2] - p1[2];

    // V2 = P2 - p1;
    v2[0] = p2[0] - p1[0];
    v2[1] = p2[1] - p1[1];
    v2[2] = p2[2] - p1[2];

    m3dCrossProduct(planeEq, v1, v2);
    m3dNormalizeVector(planeEq);
    planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}
void m3dGetPlaneEquation(M3DVector4d planeEq, const M3DVector3d p1, const M3DVector3d p2, const M3DVector3d p3)
{
    // Get two vectors do the cross product
    M3DVector3d v1, v2;

    // V1 = p3 - p1;
    v1[0] = p3[0] - p1[0];
    v1[1] = p3[1] - p1[1];
    v1[2] = p3[2] - p1[2];

    // V2 = P2 - p1;
    v2[0] = p2[0] - p1[0];
    v2[1] = p2[1] - p1[1];
    v2[2] = p2[2] - p1[2];

    m3dCrossProduct(planeEq, v1, v2);
    m3dNormalizeVector(planeEq);
    planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}

// Determine if a ray intersects a sphere
double m3dRaySphereTest(const M3DVector3d point, const M3DVector3d ray, const M3DVector3d sphereCenter,
                        double sphereRadius)
{
    M3DVector3d rayToCenter;
    m3dSubtractVectors3(rayToCenter, sphereCenter, point);
    double a = m3dDotProduct(ray, ray);
    double b = 2 * m3dDotProduct(rayToCenter, ray);
    double c = m3dDotProduct(rayToCenter, rayToCenter) - sphereRadius * sphereRadius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}
float m3dRaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter,
                       float sphereRadius)
{
    M3DVector3f rayToCenter;
    m3dSubtractVectors3(rayToCenter, sphereCenter, point);
    float a = m3dDotProduct(ray, ray);
    float b = 2 * m3dDotProduct(rayToCenter, ray);
    float c = m3dDotProduct(rayToCenter, rayToCenter) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

// Etc. etc.

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Faster (and more robust) replacements for gluProject
void m3dProjectXY(M3DVector2f vPointOut, const M3DMatrix44f mModelView, const M3DMatrix44f mProjection,
                  const int iViewPort[4], const M3DVector3f vPointIn)
{
	M3DVector4f vBack, vForth;

	memcpy(vBack, vPointIn, sizeof(M3DVector3f));
	vBack[3] = 1.0f;

	m3dTransformVector4(vForth, vBack, mModelView);
	m3dTransformVector4(vBack, vForth, mProjection);

	if (vBack[3] == 0.0f)
		return;

	vBack[0] /= vBack[3];
	vBack[1] /= vBack[3];
	vBack[2] /= vBack[3];

	vPointOut[0] = float(iViewPort[0]) + float(iViewPort[2]) * (vBack[0] + 1.0f) * 0.5f;
	vPointOut[1] = float(iViewPort[1]) + float(iViewPort[3]) * (vBack[1] + 1.0f) * 0.5f;
}
void m3dProjectXYZ(M3DVector3f vPointOut, const M3DMatrix44f mModelView, const M3DMatrix44f mProjection,
                   const int iViewPort[4], const M3DVector3f vPointIn)
{
	M3DVector4f vBack, vForth;

	memcpy(vBack, vPointIn, sizeof(M3DVector3f));
	vBack[3] = 1.0f;

	m3dTransformVector4(vForth, vBack, mModelView);
	m3dTransformVector4(vBack, vForth, mProjection);

	if (vBack[3] == 0.0f)
		return;

	vBack[0] /= vBack[3];
	vBack[1] /= vBack[3];
	vBack[2] /= vBack[3];

	vPointOut[0] = float(iViewPort[0]) + float(iViewPort[2]) * (vBack[0] + 1.0f) * 0.5f;
	vPointOut[1] = float(iViewPort[1]) + float(iViewPort[3]) * (vBack[1] + 1.0f) * 0.5f;
	vPointOut[2] = (vBack[2] + 1.0f) * 0.5f;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// This function does a three dimensional Catmull-Rom "spline" interpolation between p1 and p2
void m3dCatmullRom(M3DVector3f vOut, M3DVector3f vP0, M3DVector3f vP1, M3DVector3f vP2, M3DVector3f vP3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	// X
	vOut[0] = 0.5f * ((2.0f * vP1[0]) +
        					  (-vP0[0] + vP2[0]) * t +
        					  (2.0f * vP0[0] - 5.0f * vP1[0] + 4 * vP2[0] - vP3[0]) * t2 +
        					  (-vP0[0] + 3.0f * vP1[0] - 3.0f * vP2[0] + vP3[0]) * t3);

	// Y
	vOut[1] = 0.5f * ((2.0f * vP1[1]) +
        					  (-vP0[1] + vP2[1]) * t +
        					  (2.0f * vP0[1] - 5.0f * vP1[1] + 4 * vP2[1] - vP3[1]) * t2 +
        					  (-vP0[1] + 3.0f * vP1[1] - 3.0f * vP2[1] + vP3[1]) * t3);

	// Z
	vOut[2] = 0.5f * ((2.0f * vP1[2]) +
        					  (-vP0[2] + vP2[2]) * t +
        					  (2.0f * vP0[2] - 5.0f * vP1[2] + 4 * vP2[2] - vP3[2]) * t2 +
        					  (-vP0[2] + 3.0f * vP1[2] - 3.0f * vP2[2] + vP3[2]) * t3);
}
void m3dCatmullRom(M3DVector3d vOut, M3DVector3d vP0, M3DVector3d vP1, M3DVector3d vP2, M3DVector3d vP3, double t)
{
    double t2 = t * t;
    double t3 = t2 * t;

    // X
    vOut[0] = 0.5f * ((2.0f * vP1[0]) +
        (-vP0[0] + vP2[0]) * t +
        (2.0f * vP0[0] - 5.0f * vP1[0] + 4 * vP2[0] - vP3[0]) * t2 +
        (-vP0[0] + 3.0f * vP1[0] - 3.0f * vP2[0] + vP3[0]) * t3);

    // Y
    vOut[1] = 0.5f * ((2.0f * vP1[1]) +
        (-vP0[1] + vP2[1]) * t +
        (2.0f * vP0[1] - 5.0f * vP1[1] + 4 * vP2[1] - vP3[1]) * t2 +
        (-vP0[1] + 3.0f * vP1[1] - 3.0f * vP2[1] + vP3[1]) * t3);

    // Z
    vOut[2] = 0.5f * ((2.0f * vP1[2]) +
        (-vP0[2] + vP2[2]) * t +
        (2.0f * vP0[2] - 5.0f * vP1[2] + 4 * vP2[2] - vP3[2]) * t2 +
        (-vP0[2] + 3.0f * vP1[2] - 3.0f * vP2[2] + vP3[2]) * t3);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Compare floats and doubles... 
inline bool m3dCloseEnough(float fCandidate, float fCompare, float fEpsilon)
{
    return (fabs(fCandidate - fCompare) < fEpsilon);
}

inline bool m3dCloseEnough(double dCandidate, double dCompare, double dEpsilon)
{
    return (fabs(dCandidate - dCompare) < dEpsilon);
}

////////////////////////////////////////////////////////////////////////////
// Used for normal mapping. Finds the tangent bases for a triangle...
// Only a floating point implementation is provided.
void m3dCalculateTangentBasis(const M3DVector3f pvTriangle[3], const M3DVector2f pvTexCoords[3], const M3DVector3f N,
                              M3DVector3f vTangent)
{
	M3DVector3f dv2v1, dv3v1;
	float dc2c1t, dc2c1b, dc3c1t, dc3c1b;
	float M;

	m3dSubtractVectors3(dv2v1, pvTriangle[1], pvTriangle[0]);
	m3dSubtractVectors3(dv3v1, pvTriangle[2], pvTriangle[0]);

	dc2c1t = pvTexCoords[1][0] - pvTexCoords[0][0];
	dc2c1b = pvTexCoords[1][1] - pvTexCoords[0][1];
	dc3c1t = pvTexCoords[2][0] - pvTexCoords[0][0];
	dc3c1b = pvTexCoords[2][1] - pvTexCoords[0][1];

	M = (dc2c1t * dc3c1b) - (dc3c1t * dc2c1b);
	M = 1.0f / M;

	m3dScaleVector3(dv2v1, dc3c1b);
	m3dScaleVector3(dv3v1, dc2c1b);

	m3dSubtractVectors3(vTangent, dv2v1, dv3v1);
	m3dScaleVector3(vTangent, M); // This potentially changes the direction of the vector
	m3dNormalizeVector(vTangent);

	// Bitangent is now the cross product of the normal and tangent
	m3dCrossProduct(vTangent, N, vTangent);
}

////////////////////////////////////////////////////////////////////////////
// Smoothly step between 0 and 1 between edge1 and edge 2
double m3dSmoothStep(double edge1, double edge2, double x)
{
	double t;
	t = (x - edge1) / (edge2 - edge1);
	if (t > 1.0)
		t = 1.0;
	if (t < 0.0)
		t = 0.0;
	return t * t * (3.0 - 2.0 * t);
}
float m3dSmoothStep(float edge1, float edge2, float x)
{
	float t;
	t = (x - edge1) / (edge2 - edge1);
	if (t > 1.0f)
		t = 1.0f;
	if (t < 0.0f)
		t = 0.0f;
	return t * t * (3.0f - 2.0f * t);
}

/////////////////////////////////////////////////////////////////////////////
// Planar shadow Matrix
void m3dMakePlanarShadowMatrix(M3DMatrix44d proj, const M3DVector4d planeEq, const M3DVector3d vLightPos)
{
    // These just make the code below easier to read. They will be
    // removed by the optimizer.
    double a = planeEq[0];
    double b = planeEq[1];
    double c = planeEq[2];
    double d = planeEq[3];

    double dx = -vLightPos[0];
    double dy = -vLightPos[1];
    double dz = -vLightPos[2];

    // Now build the projection matrix
    proj[0] = b * dy + c * dz;
    proj[1] = -a * dy;
    proj[2] = -a * dz;
    proj[3] = 0.0;

    proj[4] = -b * dx;
    proj[5] = a * dx + c * dz;
    proj[6] = -b * dz;
    proj[7] = 0.0;

    proj[8] = -c * dx;
    proj[9] = -c * dy;
    proj[10] = a * dx + b * dy;
    proj[11] = 0.0;

    proj[12] = -d * dx;
    proj[13] = -d * dy;
    proj[14] = -d * dz;
    proj[15] = a * dx + b * dy + c * dz;
}
void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos)
{
    // These just make the code below easier to read. They will be
    // removed by the optimizer.
    float a = planeEq[0];
    float b = planeEq[1];
    float c = planeEq[2];
    float d = planeEq[3];

    float dx = -vLightPos[0];
    float dy = -vLightPos[1];
    float dz = -vLightPos[2];

    // Now build the projection matrix
    proj[0] = b * dy + c * dz;
    proj[1] = -a * dy;
    proj[2] = -a * dz;
    proj[3] = 0.0f;

    proj[4] = -b * dx;
    proj[5] = a * dx + c * dz;
    proj[6] = -b * dz;
    proj[7] = 0.0f;

    proj[8] = -c * dx;
    proj[9] = -c * dy;
    proj[10] = a * dx + b * dy;
    proj[11] = 0.0f;

    proj[12] = -d * dx;
    proj[13] = -d * dy;
    proj[14] = -d * dz;
    proj[15] = a * dx + b * dy + c * dz;
}

double m3dClosestPointOnRay(M3DVector3d vPointOnRay, const M3DVector3d vRayOrigin, const M3DVector3d vUnitRayDir,
                            const M3DVector3d vPointInSpace)
{
	M3DVector3d v;
	m3dSubtractVectors3(v, vPointInSpace, vRayOrigin);
    double t = m3dDotProduct(vUnitRayDir, v);
	m3dScaleVector3(v, t);
	m3dAddVectors3(vPointOnRay, vRayOrigin, v);
	return t;
}

float m3dClosestPointOnRay(M3DVector3f vPointOnRay, const M3DVector3f vRayOrigin, const M3DVector3f vUnitRayDir,
                           const M3DVector3f vPointInSpace)
{
    M3DVector3f v;
    m3dSubtractVectors3(v, vPointInSpace, vRayOrigin);
    float t = m3dDotProduct(vUnitRayDir, v);
    m3dScaleVector3(v, t);
    m3dAddVectors3(vPointOnRay, vRayOrigin, v);
    return t;
}

#endif
