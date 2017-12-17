#include "drawable.h"
#include <la.h>

Drawable::Drawable(OpenGLContext* context)
    : bufIdx(), bufPos(), bufNor(), bufCol(), bufUV(), bufBlock(),
      idxBound(false), posBound(false), norBound(false), colBound(false),
      uvBound(false), blockBound(false),
      context(context)
{}

Drawable::~Drawable()
{}


void Drawable::destroy()
{
    context->glDeleteBuffers(1, &bufIdx);
    context->glDeleteBuffers(1, &bufPos);
    context->glDeleteBuffers(1, &bufNor);
    context->glDeleteBuffers(1, &bufCol);
    context->glDeleteBuffers(1, &bufUV);
    context->glDeleteBuffers(1, &bufBlock);
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return GL_TRIANGLES;
}

int Drawable::elemCount()
{
    return count;
}

void Drawable::generateIdx()
{
    idxBound = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    context->glGenBuffers(1, &bufIdx);
}

void Drawable::generatePos()
{
    posBound = true;
    // Create a VBO on our GPU and store its handle in bufPos
    context->glGenBuffers(1, &bufPos);
}

void Drawable::generateNor()
{
    norBound = true;
    // Create a VBO on our GPU and store its handle in bufNor
    context->glGenBuffers(1, &bufNor);
}

void Drawable::generateCol()
{
    colBound = true;
    // Create a VBO on our GPU and store its handle in bufCol
    context->glGenBuffers(1, &bufCol);
}

void Drawable::generateUV()

{

    uvBound = true;

    // Create a VBO on our GPU and store its handle in bufUV

    context->glGenBuffers(1, &bufUV);

}

void Drawable::generateBlocks()

{

    blockBound = true;

    // Create a VBO on our GPU and store its handle in bufBlock

    context->glGenBuffers(1, &bufBlock);

}

void Drawable::generateData() {
    dataBound = true;

    context->glGenBuffers(1, &bufData);
}

void Drawable::generateDataTrans() {
    dataTransBound = true;

    context->glGenBuffers(1, &bufDataTrans);
}

bool Drawable::bindIdx()
{
    if(idxBound) {
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    }
    return idxBound;
}

bool Drawable::bindPos()
{
    if(posBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    }
    return posBound;
}

bool Drawable::bindNor()
{
    if(norBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    }
    return norBound;
}

bool Drawable::bindCol()
{
    if(colBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    }
    return colBound;
}

bool Drawable::bindUV()

{

    if(uvBound){

        context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);

    }

    return uvBound;

}

bool Drawable::bindBlocks()

{

    if(blockBound){

        context->glBindBuffer(GL_ARRAY_BUFFER, bufBlock);

    }

    return blockBound;

}

bool Drawable::bindData() {
    if(dataBound) {
        context->glBindBuffer(GL_ARRAY_BUFFER, bufData);
    }
    return dataBound;
}

bool Drawable::bindDataTrans() {
    if(dataTransBound) {
        context->glBindBuffer(GL_ARRAY_BUFFER, bufDataTrans);
    }
    return dataTransBound;
}
