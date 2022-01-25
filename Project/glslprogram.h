#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <QOpenGLFunctions_3_3_Core>
#include "glmath.h"

class GLSLProgram : protected QOpenGLFunctions_3_3_Core{
    GLuint handle;

    bool checkShaderStatus(GLuint shader);
    bool checkProgramStatus(GLuint program);

public:
    GLSLProgram();

    bool compileShaderFromString(const std::string &, GLenum type);
    bool compileShaderFromFile(const char *filename, GLenum type);

    void setUniform(const char* name, mat4 mat);
    void setUniform(const char* name, vec3 vec);
    void setUniform(const char* name, float val);

    bool link();
    void use();
};

#endif // GLSLPROGRAM_H
