#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class BlockSelect : public Drawable
{
public:

    // int from 0 to 8 to specify block type, and position of select
    int type;

    BlockSelect(OpenGLContext* context, int type);
    virtual void create();
    GLenum drawMode() override;
};
