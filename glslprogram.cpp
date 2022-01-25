#include "glslprogram.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
#define ENDL Qt::endl
#else
#define ENDL endl
#endif

GLSLProgram::GLSLProgram() {
    initializeOpenGLFunctions();
    // uchwyt do programu, za jego pomocą indeksujemy programy
    // glCreateProgram tworzy unikalny uchwyt
    handle = glCreateProgram();
}

bool GLSLProgram::compileShaderFromFile(const char *filename, GLenum type) {
    QFile file(filename);
    if(file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        std::string shader_src = stream.readAll().toStdString();
        return compileShaderFromString(shader_src, type);
    }
    return false;
}

bool GLSLProgram::compileShaderFromString(const std::string &source, GLenum type) {
    //stworzenie uchwytu do shadera, potrzebny tymczasowo, póki nie zostanie dowiązany do programu
    GLuint sh = glCreateShader(type);
    //rzutowanie stringa na tablicę GLchar
    GLchar* srcs[] = {(GLchar*)source.c_str()};
    //przekazanie istiejącemu, ale pustemu shaderowi jego kodu
    glShaderSource(sh, 1, srcs, NULL);
    glCompileShader(sh);
    if(checkShaderStatus(sh)) {
        // dołączenie shadera do programu pod warunkiem poprawnej kompilacji
        glAttachShader(handle, sh);
        glDeleteShader(sh);
        return true;
    }
    return false;
}

bool GLSLProgram::checkShaderStatus(GLuint shader) {
    GLint stat;
    // odczytanie zmiennej statusu kompilacji
    glGetShaderiv(shader, GL_COMPILE_STATUS, &stat);
    if(stat == GL_FALSE) {
        // utworzenie wystarczająco długiej tablicy znaków i przepisanie do niej błędów
        GLchar infoLog[10240];
        glGetShaderInfoLog(shader, 10240, NULL, infoLog);
        qDebug() << "Shader ERROR: " << infoLog << ENDL;
        return false;
    }
    return true;
}

bool GLSLProgram::checkProgramStatus(GLuint program) {
    GLint stat;
    glGetProgramiv(program, GL_LINK_STATUS, &stat);
    if(stat == GL_FALSE) {
        GLchar infoLog[10240];
        glGetProgramInfoLog(program, 10240, NULL, infoLog);
        qDebug() << "Program ERROR: " << infoLog << ENDL;
        return false;
    }
    else
        qDebug() << "Program LINKED"<< ENDL;
    return true;
}

bool GLSLProgram::link() {
    // linkowanie uprzednio skompilowanych shaderów
    glLinkProgram(handle);
    return checkProgramStatus(handle);
}

void GLSLProgram::use() {
    glUseProgram(handle);
}

void GLSLProgram::setUniform(const char* name, mat4 mat) {
    GLint loc = glGetUniformLocation(handle, name);
    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat.m);
    else
        qDebug() << "Uniform variable error: " << name << ENDL;
}

void GLSLProgram::setUniform(const char* name, vec3 vec) {
    GLint loc = glGetUniformLocation(handle, name);
    if(loc != -1)
        glUniform3f(loc, vec.x, vec.y, vec.z);
    else
        qDebug() << "Uniform variable error: " << name << ENDL;
}
void GLSLProgram::setUniform(const char* name, float val) {
    GLint loc = glGetUniformLocation(handle, name);
    if(loc != -1)
        glUniform1f(loc, val);
    else
        qDebug() << "Uniform variable error: " << name << ENDL;
}
