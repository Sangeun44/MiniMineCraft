#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Crosshairs : public Drawable
{
public:


    Crosshairs(OpenGLContext* context);
    virtual void create();
    GLenum drawMode() override;
};
