#ifndef GLMATH_H
#define GLMATH_H

#include <cmath>

struct vec3 {
    float x, y, z;
};

struct mat4 {
    float m[16];
};

inline mat4 identity() {
    return {1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
}

inline mat4 translationMat(vec3 v) {
    mat4 mat = identity();
    mat.m[12] = v.x;
    mat.m[13] = v.y;
    mat.m[14] = v.z;
    return mat;
}

inline mat4 mul(mat4 m1, mat4 m2) {
    mat4 mat;
    mat.m[0] = m1.m[0]*m2.m[0] + m1.m[4]*m2.m[1] + m1.m[8]*m2.m[2]  + m1.m[12]*m2.m[3];
    mat.m[1] = m1.m[1]*m2.m[0] + m1.m[5]*m2.m[1] + m1.m[9]*m2.m[2]  + m1.m[13]*m2.m[3];
    mat.m[2] = m1.m[2]*m2.m[0] + m1.m[6]*m2.m[1] + m1.m[10]*m2.m[2] + m1.m[14]*m2.m[3];
    mat.m[3] = m1.m[3]*m2.m[0] + m1.m[7]*m2.m[1] + m1.m[11]*m2.m[2] + m1.m[15]*m2.m[3];

    mat.m[4] = m1.m[0]*m2.m[4] + m1.m[4]*m2.m[5] + m1.m[8]*m2.m[6]  + m1.m[12]*m2.m[7];
    mat.m[5] = m1.m[1]*m2.m[4] + m1.m[5]*m2.m[5] + m1.m[9]*m2.m[6]  + m1.m[13]*m2.m[7];
    mat.m[6] = m1.m[2]*m2.m[4] + m1.m[6]*m2.m[5] + m1.m[10]*m2.m[6] + m1.m[14]*m2.m[7];
    mat.m[7] = m1.m[3]*m2.m[4] + m1.m[7]*m2.m[5] + m1.m[11]*m2.m[6] + m1.m[15]*m2.m[7];

    mat.m[8]  = m1.m[0]*m2.m[8] + m1.m[4]*m2.m[9] + m1.m[8]*m2.m[10]  + m1.m[12]*m2.m[11];
    mat.m[9]  = m1.m[1]*m2.m[8] + m1.m[5]*m2.m[9] + m1.m[9]*m2.m[10]  + m1.m[13]*m2.m[11];
    mat.m[10] = m1.m[2]*m2.m[8] + m1.m[6]*m2.m[9] + m1.m[10]*m2.m[10] + m1.m[14]*m2.m[11];
    mat.m[11] = m1.m[3]*m2.m[8] + m1.m[7]*m2.m[9] + m1.m[11]*m2.m[10] + m1.m[15]*m2.m[11];

    mat.m[12] = m1.m[0]*m2.m[12] + m1.m[4]*m2.m[13] + m1.m[8]*m2.m[14]  + m1.m[12]*m2.m[15];
    mat.m[13] = m1.m[1]*m2.m[12] + m1.m[5]*m2.m[13] + m1.m[9]*m2.m[14]  + m1.m[13]*m2.m[15];
    mat.m[14] = m1.m[2]*m2.m[12] + m1.m[6]*m2.m[13] + m1.m[10]*m2.m[14] + m1.m[14]*m2.m[15];
    mat.m[15] = m1.m[3]*m2.m[12] + m1.m[7]*m2.m[13] + m1.m[11]*m2.m[14] + m1.m[15]*m2.m[15];
    return mat;
}

inline mat4 perspectiveMat(float fovy, float aspect, float znear, float zfar) {
    mat4 mat;
    float h = tan(fovy*0.017453293);
    float w = h / aspect;
    float depth = znear - zfar;
    float q = (zfar + znear) / depth;
    float qn = 2 * zfar * znear / depth;

    mat.m[0]  = w;  mat.m[1]  = 0;  mat.m[2]  = 0;  mat.m[3]  = 0;
    mat.m[4]  = 0;  mat.m[5]  = h;  mat.m[6]  = 0;  mat.m[7]  = 0;
    mat.m[8]  = 0;  mat.m[9]  = 0;  mat.m[10] = q;  mat.m[11] = -1;
    mat.m[12] = 0;  mat.m[13] = 0;  mat.m[14] = qn;  mat.m[15] = 0;

    return mat;
}

inline mat4 rotationMat(float angle, float x, float y, float z) {
    angle *= M_PI/180.;
    mat4 mat = identity();
    float s = sin(angle);
    float c = cos(angle);

    mat.m[0] = x*x*(1-c)+c;
    mat.m[1] = y*x*(1-c)+z*s;
    mat.m[2] = x*z*(1-c)-y*s;

    mat.m[4] = x*y*(1-c)-z*s;
    mat.m[5] = y*y*(1-c)+c;
    mat.m[6] = y*z*(1-c)+x*s;

    mat.m[8] = x*z*(1-c)+y*s;
    mat.m[9] = y*z*(1-c)-x*s;
    mat.m[10] = z*z*(1-c)+c;

    return mat;
}

inline vec3 cross(vec3 v1, vec3 v2) {
    vec3 result;
    result.x = v1.y*v2.z - v1.z*v2.y;
    result.y = -v1.x*v2.z + v1.z*v2.x;
    result.z = v1.x*v2.y - v1.y*v2.x;
    return result;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return {v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}

inline vec3 operator*(const vec3 &v, const float s) {
    vec3 result;
    result.x=v.x*s;
    result.y=v.y*s;
    result.z=v.z*s;
    return result;
}

inline vec3 operator*(const vec3 &v, const mat4 &mat) {
    vec3 result;
    result.x = v.x*mat.m[0] + v.y*mat.m[4] + v.z*mat.m[8]  + mat.m[12];
    result.y = v.x*mat.m[1] + v.y*mat.m[5] + v.z*mat.m[9]  + mat.m[13];
    result.z = v.x*mat.m[2] + v.y*mat.m[6] + v.z*mat.m[10] + mat.m[14];
    return result;
}

inline float len(const vec3& v) {
    return std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

inline vec3 normal(const vec3& v) {
    float l = len(v);
    return vec3{v.x/l, v.y/l, v.z/l};
}



#endif // GLMATH_H
