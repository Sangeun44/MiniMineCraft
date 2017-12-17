#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Inventory : public Drawable
{
public:


    Inventory(OpenGLContext* context);
    virtual void create();
};

