#include "mesh.h"
#include "gldebug.h"
#include <QFile>
#include <limits>

Mesh::Mesh(GLenum primitiveType) : primitiveType(primitiveType) {
    initializeOpenGLFunctions();
    // przygotowanie vertex array object - identyfikatora dla siatki
    glGenVertexArrays(1, &vao);
}
void Mesh::setAttribute(Mesh::BufferType index, vec3 *data, int n) {
    // sprawdzenie, czy atrybut jest nowy, czy należy go nadpisać
    if(!bufferObjects.contains(index)) {
        GLuint vbo;
        // tworzenie nowego vertex buffer object dodanie jego indeksu do mapy
        glGenBuffers(1, &vbo);
        bufferObjects.insert(index, vbo);
    }

    // ustawienie aktywnego mesha
    glBindVertexArray(vao);
        // ustawienie aktywnego bufora
        glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[index]);
            // włączenie możliwości zapisu tablicy pod wskazanym indeksem
            glEnableVertexAttribArray(index);
            // przepisanie danych do bufora
            glBufferData(GL_ARRAY_BUFFER, n*sizeof(vec3), data, GL_STATIC_DRAW);
            // ustawienie lokalizacji danych w buforze, dzięki temu po indeksie
            // będzie można odszukiwać zmienne w shaderze i wiadomo jakiego będą typu
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
        // rozłączenie bufora a następnie vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    PRINT_GL_ERRORS("Mesh::setAttribute");
}

// szczególny przypadek setattribute ze względu na liczbę werteksów, która potem jest potrzebna
void Mesh::setVertices(vec3 *data, int n) {
    nVerts = n;
    setAttribute(Mesh::Vertices, data, n);
}

void Mesh::setIndices(ushort *data, uint n) {
    nInd = n;

    if(!bufferObjects.contains(Mesh::Indices)) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        bufferObjects.insert(Mesh::Indices, vbo);
    }

    glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[Mesh::Indices]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, n*sizeof(ushort), data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

// ustawienie aktywnego mesha, a następnie wyrysowanie go
void Mesh::render() {
    glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[Mesh::Indices]);
            glDrawElements(primitiveType, nInd, GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glDrawArrays(primitiveType, 0, nVerts);
    glBindVertexArray(0);

    PRINT_GL_ERRORS("Mesh::render");

}

void Mesh::setMinCoord(vec3 minCoord)
{
    this->minCoord = minCoord;
}

void Mesh::setMaxCoord(vec3 maxCoord)
{
    this->maxCoord = maxCoord;
}

vec3 Mesh::getMinCoord()
{
    return minCoord;
}

vec3 Mesh::getMaxCoord()
{
    return maxCoord;
}

Mesh *Mesh::loadFromObj(const char *filename) {
    QFile file(filename);
    float FLOAT_MAX_NUM = std::numeric_limits<float>::max();
    float FLOAT_MIN_NUM = std::numeric_limits<float>::min();
    vec3 minVec = {FLOAT_MAX_NUM, FLOAT_MAX_NUM, FLOAT_MAX_NUM };
    vec3 maxVec = {FLOAT_MIN_NUM, FLOAT_MIN_NUM, FLOAT_MIN_NUM};
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    //mesh->name = filename;
    if(file.open(QFile::ReadOnly)) {
        int currentIndex=0;
        QTextStream stream(&file);
        std::vector<vec3> vertices, normals, resultVertices, resultNormals;
        std::vector<ushort> indices;
        while(!stream.atEnd()) {
            QString line = stream.readLine();
            if(!line.isEmpty()) {
                auto listElements = line.split(' ');
                if(line[0]=='v') {
                    vec3 vertex = {listElements[1].toFloat(), listElements[2].toFloat(), listElements[3].toFloat()};
                    if(line[1]=='n')
                        normals.push_back(vertex);
                    else
                    {
                        vertices.push_back(vertex);
                    }
                }
                else if(line[0]=='f') {
                    for(int i=1; i<=3; i++) {
                        auto faceData = listElements[i].split('/');
                        resultVertices.push_back(vertices[faceData[0].toUShort()-1]);
                        resultNormals.push_back(normals[faceData[2].toUShort()-1]);
                        indices.push_back(currentIndex++);
                    }
                }
            }
        }
        for(int i = 0; i < resultVertices.size(); i++)
        {
            vec3 vertex = resultVertices[i];
            if(minVec.x >= vertex.x)
                minVec.x = vertex.x;

            if(minVec.y >= vertex.y)
                minVec.y = vertex.y;

            if(minVec.z >= vertex.z)
                minVec.z = vertex.z;

            if(maxVec.x <= vertex.x)
                maxVec.x = vertex.x;

            if(maxVec.y <= vertex.y)
                maxVec.y = vertex.y;

            if(maxVec.z <= vertex.z)
                maxVec.z = vertex.z;
        }
        mesh->setVertices(resultVertices.data(), resultVertices.size());
        mesh->setAttribute(Mesh::Normals, resultNormals.data(), resultNormals.size());
        mesh->setIndices(indices.data(), indices.size());
        mesh->setMaxCoord(maxVec);
        mesh->setMinCoord(minVec);
        file.close();
        qDebug() << filename << " loaded";
    }
    else
        qDebug() << filename << " loading error";
    return mesh;
}

//Mesh *Mesh::loadFromObj(const char *filename) {
//    QFile file(filename);
//    Mesh *mesh = new Mesh(GL_TRIANGLES);
//    //mesh->name = filename;
//    if(file.open(QFile::ReadOnly)) {
//        QTextStream stream(&file);
//        std::vector<vec3> vertices;
//        std::vector<ushort> indices;
//        while(!stream.atEnd()) {
//            QString line = stream.readLine();
//            if(!line.isEmpty()) {
//                auto listElements = line.split(' ');
//                if(line[0]=='v') {
//                    vec3 vertex = {listElements[1].toFloat(), listElements[2].toFloat(), listElements[3].toFloat()};
//                    vertices.push_back(vertex);
//                }
//                else if(line[0]=='f') {
//                    indices.push_back(listElements[1].toUShort()-1);
//                    indices.push_back(listElements[2].toUShort()-1);
//                    indices.push_back(listElements[3].toUShort()-1);
//                }
//            }
//        }
//        mesh->setVertices(vertices.data(), vertices.size());
//        mesh->setIndices(indices.data(), indices.size());
//        file.close();
//        qDebug() << filename << " loaded";
//    }
//    else
//        qDebug() << filename << " loading error";
//    return mesh;
//}
