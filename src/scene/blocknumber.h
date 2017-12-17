#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class BlockNumber : public Drawable
{
public:

    int number;
    glm::vec2 bottomRightCorner;

    BlockNumber(OpenGLContext* context, glm::vec2 corner, int number);
    virtual void create();
};
