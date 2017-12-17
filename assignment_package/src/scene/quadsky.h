#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class QuadSky : public Drawable
{
public:
    QuadSky(OpenGLContext* context);
    virtual void create();
};
