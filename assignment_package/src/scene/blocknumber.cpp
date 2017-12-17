#include "blocknumber.h"

BlockNumber::BlockNumber(OpenGLContext *context, glm::vec2 corner, int number) : Drawable(context), bottomRightCorner(corner), number(number)
{}

void BlockNumber::create()
{
    GLuint idx[6]{0, 1, 2, 0, 2, 3};
    glm::vec4 vert_pos[4] {glm::vec4(bottomRightCorner.x - .1f, bottomRightCorner.y, .000000000001f, 1.f),
                           glm::vec4(bottomRightCorner.x, bottomRightCorner.y, .000000000001f, 1.f),
                           glm::vec4(bottomRightCorner.x, bottomRightCorner.y + .15f, .000000000001f, 1.f),
                           glm::vec4(bottomRightCorner.x - .1f, bottomRightCorner.y + .15f, .000000000001f, 1.f)};

    std::vector<glm::vec2> vert_UV;
    switch(number) {
    case 0:
        vert_UV.push_back(glm::vec2(12.f / 16.f, 5.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 5.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 6.f / 16.f));
        vert_UV.push_back(glm::vec2(12.f / 16.f, 6.f / 16.f));
        break;
    case 1:
        vert_UV.push_back(glm::vec2(12.f / 16.f, 4.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 5.f / 16.f));
        vert_UV.push_back(glm::vec2(12.f / 16.f, 5.f / 16.f));
        break;
    case 2:
        vert_UV.push_back(glm::vec2(12.f / 16.f, 3.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
        vert_UV.push_back(glm::vec2(12.f / 16.f, 4.f / 16.f));
        break;
    case 3:
        vert_UV.push_back(glm::vec2(12.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
        vert_UV.push_back(glm::vec2(12.f / 16.f, 3.f / 16.f));
        break;
    case 4:
        vert_UV.push_back(glm::vec2(12.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(12.f / 16.f, 2.f / 16.f));
        break;
    case 5:
        vert_UV.push_back(glm::vec2(11.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(12.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(12.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(11.f / 16.f, 2.f / 16.f));
        break;
    case 6:
        vert_UV.push_back(glm::vec2(10.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(11.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(11.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(10.f / 16.f, 2.f / 16.f));
        break;
    case 7:
        vert_UV.push_back(glm::vec2(9.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(10.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(10.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(9.f / 16.f, 2.f / 16.f));
        break;
    case 8:
        vert_UV.push_back(glm::vec2(8.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(9.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(9.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(8.f / 16.f, 2.f / 16.f));
        break;
    case 9:
        vert_UV.push_back(glm::vec2(7.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(8.f / 16.f, 1.f / 16.f));
        vert_UV.push_back(glm::vec2(8.f / 16.f, 2.f / 16.f));
        vert_UV.push_back(glm::vec2(7.f / 16.f, 2.f / 16.f));
        break;
    }

    count = 6;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec4), vert_pos, GL_STATIC_DRAW);

    generateUV();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);
    context->glBufferData(GL_ARRAY_BUFFER, vert_UV.size() * sizeof(glm::vec2), vert_UV.data(), GL_STATIC_DRAW);
}
