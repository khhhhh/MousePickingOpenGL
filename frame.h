#ifndef FRAME_H
#define FRAME_H

#include "glmath.h"

class Frame
{
public:
    vec3 pos, up, forward;
    virtual vec3 x() const;
    virtual mat4 matrix() const;

    Frame();
};

#endif // FRAME_H
