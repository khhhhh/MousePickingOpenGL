#include "camera.h"

Camera::Camera() {
    pos = {2.5,1,10};
    up = {0,1,0};
    forward = {0,0,-1};
}

vec3 Camera::x() const{
    return cross(forward, up);
}

mat4 Camera::matrix() const {
    mat4 result = identity();
    vec3 vecX = x();
    vec3 upp = cross(vecX, forward);
    result.m[0] = vecX.x;    result.m[1] = upp.x;    result.m[2]  = -forward.x;
    result.m[4] = vecX.y;    result.m[5] = upp.y;    result.m[6]  = -forward.y;
    result.m[8] = vecX.z;    result.m[9] = upp.z;    result.m[10] = -forward.z;

    result = mul(result, translationMat(vec3{-pos.x,-pos.y,-pos.z}));

    return result;
}
