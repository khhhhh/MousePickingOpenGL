#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>

#include "mesh.h"
#include "glslprogram.h"
#include "camera.h"
#include <set>

class OpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    QTimer timer;
    Camera *camera;
    std::vector<Mesh *> meshes;
    GLSLProgram *program, *gourardProgram, *phongProgram;

    mat4 projMat;

    QPoint refPoint;


    int de;
    float angle;
    float ax,ay,dax,day;
    vec3 lightPosition;

    int winWidth, winHeight;


    void processCamera();
    void rotateLight();
    void switchProgram();

    bool glowObject(vec3 coords);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePicking(float ax, float ay);

public:
    std::set<int> keys;

    ~OpenGlWidget();
};
#endif // WIDGET_H
