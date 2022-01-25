#ifndef CAMERA_H
#define CAMERA_H

#include "frame.h"

class Camera : public Frame
{
public:
    Camera();
    vec3 x() const;
    mat4 matrix() const;
};

#endif // CAMERA_H
