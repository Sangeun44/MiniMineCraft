#include "blockselect.h"

BlockSelect::BlockSelect(OpenGLContext* context, int type)
    : Drawable(context), type(type)
{}

void BlockSelect::create() {
    GLuint idx[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    glm::vec4 pos[8] = {glm::vec4(.0f,.0f, .000000000001f,1),
                        glm::vec4(.0f,.0f, .000000000001f,1),
                        glm::vec4(.0f,.0f, .000000000001f,1),
                        glm::vec4(.0f,.0f, .000000000001f,1),
                        glm::vec4(.0f,.0f, .000000000001f,1),
                        glm::vec4(.0f,.0f, .000000000001f,1),
                        glm::vec4(.0f,.0f, .000000000001f,1),
                        glm::vec4(.0f,.0f, .000000000001f,1)};

    switch(type) {
    case 0:
        pos[0].x = -.23f;
        pos[0].y = .31f;

        pos[1].x = -.095f;
        pos[1].y = .31f;

        pos[2].x = -.095f;
        pos[2].y = .31f;

        pos[3].x = -.095f;
        pos[3].y = .13f;

        pos[4].x = -.095f;
        pos[4].y = .13f;

        pos[5].x = -.23f;
        pos[5].y = .13f;

        pos[6].x = -.23f;
        pos[6].y = .13f;

        pos[7].x = -.23f;
        pos[7].y = .31f;

        break;
    case 1:
        pos[0].x = -.23f + .158f;
        pos[0].y = .31f;

        pos[1].x = -.095f + .158f;
        pos[1].y = .31f;

        pos[2].x = -.095f + .158f;
        pos[2].y = .31f;

        pos[3].x = -.095f + .158f;
        pos[3].y = .13f;

        pos[4].x = -.095f + .158f;
        pos[4].y = .13f;

        pos[5].x = -.23f + .158f;
        pos[5].y = .13f;

        pos[6].x = -.23f + .158f;
        pos[6].y = .13f;

        pos[7].x = -.23f + .158f;
        pos[7].y = .31f;

        break;
    case 2:
        pos[0].x = -.23f + .158f + .158f;
        pos[0].y = .31f;

        pos[1].x = -.095f + .158f + .158f;
        pos[1].y = .31f;

        pos[2].x = -.095f + .158f + .158f;
        pos[2].y = .31f;

        pos[3].x = -.095f + .158f + .158f;
        pos[3].y = .13f;

        pos[4].x = -.095f + .158f + .158f;
        pos[4].y = .13f;

        pos[5].x = -.23f + .158f + .158f;
        pos[5].y = .13f;

        pos[6].x = -.23f + .158f + .158f;
        pos[6].y = .13f;

        pos[7].x = -.23f + .158f + .158f;
        pos[7].y = .31f;

        break;
    case 3:
        pos[0].x = -.23f;
        pos[0].y = .31f - .213f;

        pos[1].x = -.095f;
        pos[1].y = .31f - .213f;

        pos[2].x = -.095f;
        pos[2].y = .31f - .213f;

        pos[3].x = -.095f;
        pos[3].y = .13f - .213f;

        pos[4].x = -.095f;
        pos[4].y = .13f - .213f;

        pos[5].x = -.23f;
        pos[5].y = .13f - .213f;

        pos[6].x = -.23f;
        pos[6].y = .13f - .213f;

        pos[7].x = -.23f;
        pos[7].y = .31f - .213f;

        break;
    case 4:
        pos[0].x = -.23f + .158f;
        pos[0].y = .31f - .213f;

        pos[1].x = -.095f + .158f;
        pos[1].y = .31f - .213f;

        pos[2].x = -.095f + .158f;
        pos[2].y = .31f - .213f;

        pos[3].x = -.095f + .158f;
        pos[3].y = .13f - .213f;

        pos[4].x = -.095f + .158f;
        pos[4].y = .13f - .213f;

        pos[5].x = -.23f + .158f;
        pos[5].y = .13f - .213f;

        pos[6].x = -.23f + .158f;
        pos[6].y = .13f - .213f;

        pos[7].x = -.23f + .158f;
        pos[7].y = .31f - .213f;

        break;
    case 5:
        pos[0].x = -.23f + .158f + .158f;
        pos[0].y = .31f - .213f;

        pos[1].x = -.095f + .158f + .158f;
        pos[1].y = .31f - .213f;

        pos[2].x = -.095f + .158f + .158f;
        pos[2].y = .31f - .213f;

        pos[3].x = -.095f + .158f + .158f;
        pos[3].y = .13f - .213f;

        pos[4].x = -.095f + .158f + .158f;
        pos[4].y = .13f - .213f;

        pos[5].x = -.23f + .158f + .158f;
        pos[5].y = .13f - .213f;

        pos[6].x = -.23f + .158f + .158f;
        pos[6].y = .13f - .213f;

        pos[7].x = -.23f + .158f + .158f;
        pos[7].y = .31f - .213f;

        break;
    case 6:
        pos[0].x = -.23f;
        pos[0].y = .31f - .213f - .213f;

        pos[1].x = -.095f;
        pos[1].y = .31f - .213f - .213f;

        pos[2].x = -.095f;
        pos[2].y = .31f - .213f - .213f;

        pos[3].x = -.095f;
        pos[3].y = .13f - .213f - .213f;

        pos[4].x = -.095f;
        pos[4].y = .13f - .213f - .213f;

        pos[5].x = -.23f;
        pos[5].y = .13f - .213f - .213f;

        pos[6].x = -.23f;
        pos[6].y = .13f - .213f - .213f;

        pos[7].x = -.23f;
        pos[7].y = .31f - .213f - .213f;

        break;
    case 7:
        pos[0].x = -.23f + .158f;
        pos[0].y = .31f - .213f - .213f;

        pos[1].x = -.095f + .158f;
        pos[1].y = .31f - .213f - .213f;

        pos[2].x = -.095f + .158f;
        pos[2].y = .31f - .213f - .213f;

        pos[3].x = -.095 + .158f;
        pos[3].y = .13f - .213f - .213f;

        pos[4].x = -.095f + .158f;
        pos[4].y = .13f - .213f - .213f;

        pos[5].x = -.23f + .158f;
        pos[5].y = .13f - .213f - .213f;

        pos[6].x = -.23f + .158f;
        pos[6].y = .13f - .213f - .213f;

        pos[7].x = -.23f + .158f;
        pos[7].y = .31f - .213f - .213f;

        break;
    case 8:
        pos[0].x = -.23f + .158f + .158f;
        pos[0].y = .31f - .213f - .213f;

        pos[1].x = -.095f + .158f + .158f;
        pos[1].y = .31f - .213f - .213f;

        pos[2].x = -.095f + .158f + .158f;
        pos[2].y = .31f - .213f - .213f;

        pos[3].x = -.095f + .158f + .158f;
        pos[3].y = .13f - .213f - .213f;

        pos[4].x = -.095f + .158f + .158f;
        pos[4].y = .13f - .213f - .213f;

        pos[5].x = -.23f + .158f + .158f;
        pos[5].y = .13f - .213f - .213f;

        pos[6].x = -.23f + .158f + .158f;
        pos[6].y = .13f - .213f - .213f;

        pos[7].x = -.23f + .158f + .158f;
        pos[7].y = .31f - .213f - .213f;

        break;
    }

    glm::vec4 col[8] = {glm::vec4(1,1,0,1),
                        glm::vec4(1,1,0,1),
                        glm::vec4(1,1,0,1),
                        glm::vec4(1,1,0,1),
                        glm::vec4(1,1,0,1),
                        glm::vec4(1,1,0,1),
                        glm::vec4(1,1,0,1),
                        glm::vec4(1,1,0,1)};

    count = 8;

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * sizeof(GLuint), idx, GL_STATIC_DRAW);
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(glm::vec4), pos, GL_STATIC_DRAW);
    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(glm::vec4), col, GL_STATIC_DRAW);
}

GLenum BlockSelect::drawMode() {
    return GL_LINES;
}
