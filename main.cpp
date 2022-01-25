#include "widget.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    QSurfaceFormat::setDefaultFormat(format);

    OpenGlWidget w;
    w.show();
    return a.exec();
}
