#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Quad : public Drawable
{
public:
    // true is water
    // false is lava
    bool type;

    Quad(OpenGLContext* context);
    virtual void create();
};
