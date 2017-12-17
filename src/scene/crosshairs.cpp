#include "crosshairs.h"

Crosshairs::Crosshairs(OpenGLContext* context)
    : Drawable(context)
{}

void Crosshairs::create() {
    GLuint idx[4] = {0, 1, 2, 3};
    glm::vec4 pos[4] = {glm::vec4(0.f,-.1f,0.0001f,1),
                        glm::vec4(0.f,.1f,0.0001f,1),
                        glm::vec4(-.1f,0.f,0.0001f,1), glm::vec4(.1f,0.f,0.0001f,1)};
    glm::vec4 col[4] = {glm::vec4(1,1,1,1),
                        glm::vec4(1,1,1,1),
                        glm::vec4(1,1,1,1), glm::vec4(1,1,1,1)};

    count = 4;

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), idx, GL_STATIC_DRAW);
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), pos, GL_STATIC_DRAW);
    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), col, GL_STATIC_DRAW);
}

GLenum Crosshairs::drawMode() {
    return GL_LINES;
}
