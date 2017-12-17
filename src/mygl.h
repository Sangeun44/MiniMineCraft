#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cube.h>
#include <scene/worldaxes.h>
#include "camera.h"
#include "player.h"
#include <scene/quad.h>
#include <scene/inventory.h>
#include <scene/blockselect.h>
#include <scene/blocknumber.h>
#include <scene/terrain.h>
#include <QThreadPool>
#include "chunker.h"
#include "blockers.h"
#include <scene/quadsky.h>
#include "scene/crosshairs.h"
#include <memory>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class MyGL : public OpenGLContext
{
    Q_OBJECT
private:
    Cube* mp_geomCube;// The instance of a unit cube we can use to render any cube. Should NOT be used in final version of your project.
    WorldAxes* mp_worldAxes; // A wireframe representation of the world axes. It is hard-coded to sit centered at (32, 128, 32).
    Crosshairs* mp_crosshairs;
    ShaderProgram* mp_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram* mp_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram* mp_progSky; // A screen-space shader for creating the sky background
    ShaderProgram* mp_progOverlay;
    ShaderProgram* mp_progMenu;
    Player* mp_player; //player for the game

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    GLuint m_textureHandle;

    int64_t time;
    float timeShader;
    Terrain* mp_terrain;

    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;

    void bindAppropriateTexture(); // for texturing

    void MoveMouseToCenter(); // Forces the mouse position to the screen's center. You should call this
                              // from within a mouse move event after reading the mouse movement so that
                              // your mouse stays within the screen bounds and is always read.

    float rayBoxInt(glm::vec3 box);
    glm::vec3 rayMarching(float nearBox, float farBox);

    bool rayMarchCollision();
    bool rayMarchFloor();
    bool rayMarchCollidedJump();

    bool creativeMode;
    bool swimming;
    bool inventoryUp;

    Quad m_geomQuad; // screen space quadrangle
    QuadSky* mp_SkyQuad;
    Inventory m_inventory;
    BlockSelect m_selection;
    BlockNumber* m_0;
    BlockNumber* m_1;
    BlockNumber* m_2;
    BlockNumber* m_3;
    BlockNumber* m_4;
    BlockNumber* m_5;
    BlockNumber* m_6;
    BlockNumber* m_7;
    BlockNumber* m_8;

    BlockType currBlock;

    QList<Chunk*> currChunks;



public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void GLDrawScene();

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent * e);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();
};


#endif // MYGL_H
