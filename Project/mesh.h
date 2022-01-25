#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions_3_3_Core>
#include "glmath.h"
#include "frame.h"

class Mesh : public Frame, protected QOpenGLFunctions_3_3_Core{
public:
    enum BufferType {Vertices=0, Indices=1, Colors=2, Normals=3};

    struct Material {
        vec3 ambient, diffuse, specular;
        float shiness;

        Material() {
            ambient = {0.5, 0.5, 0.5};
            diffuse = {0.5, 0.5, 0.5};
            specular = {0.5, 0.5, 0.5};
            shiness = 50;
        }
    };

private:
    QString name;
    uint nVerts, nInd;
    GLuint vao;
    GLenum primitiveType;
    QMap<BufferType, GLuint> bufferObjects;
    vec3 minCoord;
    vec3 maxCoord;

public:
    Mesh(GLenum primitiveType);
    void setAttribute(Mesh::BufferType, vec3 *data, int n);
    void setVertices(vec3 *data, int n);
    void setIndices(ushort *data, uint n);
    void render();
    void setMinCoord(vec3 minCoord);
    void setMaxCoord(vec3 maxCoord);
    vec3 getMinCoord();
    vec3 getMaxCoord();

    static Mesh *loadFromObj(const char *filename);

    Material material;
};

#endif // MESH_H
