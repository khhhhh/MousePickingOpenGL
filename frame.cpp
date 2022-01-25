#include "frame.h"

Frame::Frame() {
    pos = {0,0,0};
    up = {0,1,0};
    forward = {0,0,1};

}

vec3 Frame::x() const{
    return cross(up, forward);
}

mat4 Frame::matrix() const
{
    mat4 result = identity();
    vec3 vecX = x();
    vec3 upp = cross(forward, vecX);
    result.m[0] = vecX.x;    result.m[4] = upp.x;    result.m[8]  = forward.x;
    result.m[1] = vecX.y;    result.m[5] = upp.y;    result.m[9]  = forward.y;
    result.m[2] = vecX.z;    result.m[6] = upp.z;    result.m[10] = forward.z;

    result.m[12] = pos.x;
    result.m[13] = pos.y;
    result.m[14] = pos.z;

    return result;
}
