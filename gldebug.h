#ifndef GLDEBUG_H
#define GLDEBUG_H

#include <GL/glu.h>
#include <QDebug>

// odczytywanie błędów ze stosu, glGetError zwraca błąd i usuwa go ze stosu
inline void PRINT_GL_ERRORS(const char* mess) {
  GLenum err;
  /*
  while( (err=glGetError()) != GL_NO_ERROR) {
      qDebug() << "ERROR in: " << mess << (const char*)gluErrorString(err);
  }
  */
}

#endif // GLDEBUG_H
