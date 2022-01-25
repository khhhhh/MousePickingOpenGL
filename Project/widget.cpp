#include "widget.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFile>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


OpenGlWidget::~OpenGlWidget()
{
}

void OpenGlWidget::initializeGL() {
    de = 0;
    angle = 0.0f;
    ax=ay=dax=day=0;

    lightPosition = {10, 5, 0};
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    camera = new Camera();
    Mesh **meshArray = new Mesh*[4];

    for(int i = 0; i < 4; i++)
    {
        meshArray[i] = Mesh::loadFromObj(":/monkey.obj");

        vec3 max = meshArray[i]->getMaxCoord();
        meshArray[i]->pos = {-2 + (float)(i * max.x * 2 ), 0, -1};
        meshes.push_back(meshArray[i]);

        meshArray[i]->material.ambient = vec3{ 0.329412f, 0.223529f, 0.027451f };
        meshArray[i]->material.diffuse = vec3{ 0.780392f, 0.568627f, 0.113725f };
        meshArray[i]->material.specular = vec3{ 0.992157f, 0.941176f, 0.807843f };
        meshArray[i]->material.shiness = 24.0f;
    }

    gourardProgram = new GLSLProgram();
    gourardProgram->compileShaderFromFile(":/vshader.vert", GL_VERTEX_SHADER);
    gourardProgram->compileShaderFromFile(":/fshader.fsh", GL_FRAGMENT_SHADER);
    gourardProgram->link();

    /*
    phongProgram = new GLSLProgram();
    phongProgram->compileShaderFromFile("phong.vert", GL_VERTEX_SHADER);
    phongProgram->compileShaderFromFile("phong.fsh", GL_FRAGMENT_SHADER);
    phongProgram->link();
    */

    program = gourardProgram;

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.setInterval(10);
    timer.start();
}
void OpenGlWidget::paintGL() {
    // ustawienie koloru tła na biały
    processCamera();
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    program->use();

    program->setUniform("ViewMat", camera->matrix());
    program->setUniform("ProjMat", projMat);
    program->setUniform("LightPosition", vec3{0,2,0});



    for(size_t i=0; i<meshes.size(); i++) {
        Mesh *mesh = meshes[i];
        program->setUniform("ModelMat", mesh->matrix());

        program->setUniform("MaterialAmbient", mesh->material.ambient);
        program->setUniform("MaterialDiffuse", mesh->material.diffuse);
        program->setUniform("MaterialSpecular", mesh->material.specular);
        program->setUniform("MaterialShiness", mesh->material.shiness);

        mesh->render();
    }

}
void OpenGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    projMat = perspectiveMat(60.0f, w/(float)h, 0.01f, 15.0f);
    winWidth = w;
    winHeight = h;
}

void OpenGlWidget::keyPressEvent(QKeyEvent *event) {
    keys.insert(event->key());
}

void OpenGlWidget::keyReleaseEvent(QKeyEvent *event) {
    keys.erase(event->key());
}

void OpenGlWidget::mousePressEvent(QMouseEvent * e) {
    ax = e->pos().x();
    ay = e->pos().y();
    if(e->button() == Qt::LeftButton){
        ax = e->pos().x();
        ay = e->pos().y();
    }else if(e->button() == Qt::RightButton) {
        float *m = camera->matrix().m;
        glm::mat4 view = { m[0], m[1], m[2], m[3],
                           m[4], m[5], m[6], m[7],
                           m[8], m[9], m[10], m[11],
                           m[12], m[13], m[14], m[15]
                };
        m = projMat.m;
        glm::mat4 projection = { m[0], m[1], m[2], m[3],
                           m[4], m[5], m[6], m[7],
                           m[8], m[9], m[10], m[11],
                           m[12], m[13], m[14], m[15]
                };
        glm::vec4 viewport = glm::vec4(0, 0, winWidth, winHeight);
        glm::vec3 wincoord = glm::vec3(ax, winHeight - ay, -0.1f);
        glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);
        vec3 pos = {objcoord.x, objcoord.y, objcoord.z};
        for(size_t i = 0; i < meshes.size(); i++)
        {
            meshes[i]->material.ambient = vec3{ 0.329412f, 0.223529f, 0.027451f };
            meshes[i]->material.diffuse = vec3{ 0.780392f, 0.568627f, 0.113725f };
            meshes[i]->material.specular = vec3{ 0.992157f, 0.941176f, 0.807843f };
            meshes[i]->material.shiness = 24.0f;
        }
        for(int i = 1; i < 20; i++ )
        {
            pos =  normal(pos - camera->pos) * i + camera->pos;
            if(glowObject(pos))
                break;
        }
    }
}

void OpenGlWidget::mouseMoveEvent(QMouseEvent * e) {
    if(e->buttons() == Qt::LeftButton) {
        dax += ax-e->pos().x();
        day += ay-e->pos().y();

        ax = e->pos().x();
        ay = e->pos().y();
    }
}

void OpenGlWidget::processCamera() {
    float dv = 0.1f;

    if(keys.find(Qt::Key_W) != keys.end())
        camera->pos = camera->pos + camera->forward*dv;
    else if(keys.find(Qt::Key_S) != keys.end())
        camera->pos = camera->pos - camera->forward*dv;
    if(keys.find(Qt::Key_A) != keys.end())
        camera->pos = camera->pos - camera->x()*dv;
    else if(keys.find(Qt::Key_D) != keys.end())
        camera->pos = camera->pos + camera->x()*dv;
    if(keys.find(Qt::Key_Q) != keys.end())
        camera->pos = camera->pos + camera->up*dv;
    else if(keys.find(Qt::Key_Z) != keys.end())
        camera->pos = camera->pos - camera->up*dv;

    camera->forward = {0,0,-1};
    camera->forward = camera->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera->x());
    camera->forward = camera->forward * rotationMat(std::clamp<float>(day,-89.9,89.9), cx.x, cx.y, cx.z);
}

void OpenGlWidget::rotateLight() {
    lightPosition = lightPosition * rotationMat(1, 0,1,0);
    lightPosition = lightPosition * rotationMat(1, 0, 1, 0);
}

bool OpenGlWidget::glowObject(vec3 coords)
{
    for(size_t i = 0; i < meshes.size(); i++)
    {
        vec3 pos = meshes[i]->pos;
        vec3 min = meshes[i]->getMinCoord();
        vec3 max = meshes[i]->getMaxCoord();
        if( pos.x + min.x <= coords.x &&
            pos.y + min.y <= coords.y &&
            pos.z + min.z  <= coords.z &&
            pos.x + max.x  >= coords.x &&
            pos.y + max.y  >= coords.y &&
            pos.z + max.z  >= coords.z )
        {
            meshes[i]->material.ambient = vec3{ 0.5, 0, 0};
            meshes[i]->material.diffuse = vec3{ 1.0f, 1.0f, 1.0f };
            meshes[i]->material.specular = vec3{ 0.5, 0.5, 0.5};
            return true;
        }
    }
    return false;
}

