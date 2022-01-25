QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# LIBS += -lGLU

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    frame.cpp \
    glslprogram.cpp \
    main.cpp \
    mesh.cpp \
    widget.cpp

HEADERS += \
    camera.h \
    frame.h \
    gldebug.h \
    glmath.h \
    glslprogram.h \
    mesh.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fshader.fsh \
    phong.fsh \
    phong.vert \
    vshader.vert

RESOURCES += \
    shaders.qrc
