#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDateTime>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      mp_geomCube(new Cube(this)), mp_worldAxes(new WorldAxes(this)), mp_crosshairs(new Crosshairs(this)),
      mp_progLambert(new ShaderProgram(this)), mp_progFlat(new ShaderProgram(this)), mp_progMenu(new ShaderProgram(this)), mp_progSky(new ShaderProgram(this)),
      mp_player(new Player()), time(), mp_terrain(new Terrain()), creativeMode(false), swimming(false), inventoryUp(false),
      m_geomQuad(this), m_inventory(this), m_selection(this, 0), currChunks(QList<Chunk*>()), currBlock(GRASS),
      m_0(new BlockNumber(this, glm::vec2(-.095f, .13f), 9)),
      m_1(new BlockNumber(this, glm::vec2(-.095f + .158f, .13f), 9)),
      m_2(new BlockNumber(this, glm::vec2(-.095f + .158f + .158f, .13f), 9)),
      m_3(new BlockNumber(this, glm::vec2(-.095f, .13f - .213f), 9)),
      m_4(new BlockNumber(this, glm::vec2(-.095f + .158f, .13f - .213f), 9)),
      m_5(new BlockNumber(this, glm::vec2(-.095f + .158f + .158f, .13f - .213f), 9)),
      m_6(new BlockNumber(this, glm::vec2(-.095f, .13f - .213f - .213f), 9)),
      m_7(new BlockNumber(this, glm::vec2(-.095f + .158f, .13f - .213f - .213f), 9)),
      m_8(new BlockNumber(this, glm::vec2(-.095f + .158f + .158f, .13f - .213f - .213f), 9)),
      mp_SkyQuad(new QuadSky(this))
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    setFocusPolicy(Qt::ClickFocus);
    time = QDateTime::currentMSecsSinceEpoch();
    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::BlankCursor); // Make the cursor invisible
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    mp_geomCube->destroy();

    delete mp_geomCube;
    m_geomQuad.destroy();
    delete mp_worldAxes;
    delete mp_crosshairs;
    delete mp_progLambert;
    delete mp_progFlat;
    delete mp_progMenu;
    delete mp_player;
    delete mp_terrain;
    m_geomQuad.destroy();
    m_inventory.destroy();
    m_selection.destroy();
    delete m_0;
    delete m_1;
    delete m_2;
    delete m_3;
    delete m_4;
    delete m_5;
    delete m_6;
    delete m_7;
    delete m_8;
}


void MyGL::MoveMouseToCenter()
{
    QCursor::setPos(this->mapToGlobal(QPoint(width() / 2, height() / 2)));
}

bool MyGL::rayMarchCollidedJump() {
    glm::vec3 player = mp_player->getCamera()->eye;
    glm::vec3 r0 = glm::vec3(player.x, player.y, player.z);
    glm::vec3 rd = mp_player->getVelocity(); //current velocity in y
    rd = glm::vec3(0, rd.y, 0);

    float max = glm::length(rd);

    for (float t = 0; t < 1; t += 0.001f) {
        glm::vec3 pos = r0 + t * rd;
        pos = glm::vec3((int) pos.x, (int) pos.y, (int) pos.z);
        BlockType block = mp_terrain->getBlockAt(pos.x, pos.y, pos.z);
        if (block != EMPTY && block != WATER && block != LAVA) {
            return true;
        }
    }
    return false;
}

bool MyGL::rayMarchCollision() {
    glm::vec3 player = mp_player->getCamera()->eye;
    glm::vec3 r0 = glm::vec3(player.x, player.y - 0.4f, player.z);
    glm::vec3 rd = mp_player->getVelocity(); //current velocity in y
    rd = glm::vec3(rd.x, 0, rd.z);

    for (float t = 0; t < 1; t += 0.0001f) {
        glm::vec3 pos = r0 + t * rd;
        pos = glm::vec3((int) pos.x, (int) pos.y, (int) pos.z);

        BlockType block = mp_terrain->getBlockAt(pos.x, pos.y, pos.z);

        if (block != EMPTY && block != WATER && block != LAVA) {
            return true;

        }
    }

    player = mp_player->getCamera()->eye;
    r0 = glm::vec3(player.x, player.y - 1.f, player.z);
    rd = mp_player->getVelocity(); //current velocity in y
    rd = glm::vec3(rd.x, 0, rd.z);

    for (float t = 0; t < 1; t += 0.0001f) {
        glm::vec3 pos = r0 + t * rd;
        pos = glm::vec3((int) pos.x, (int) pos.y, (int) pos.z);
        BlockType block = mp_terrain->getBlockAt(pos.x, pos.y, pos.z);
        if (block != EMPTY && block != WATER && block != LAVA) {
            return true;
        }
    }

    player = mp_player->getCamera()->eye;
    r0 = glm::vec3(player.x, player.y - 1.5f, player.z);
    rd = mp_player->getVelocity(); //current velocity in y
    rd = glm::vec3(rd.x, 0, rd.z);

    for (float t = 0; t < 1; t += 0.0001f) {
        glm::vec3 pos = r0 + t * rd;
        pos = glm::vec3((int) pos.x, (int) pos.y, (int) pos.z);
        BlockType block = mp_terrain->getBlockAt(pos.x, pos.y, pos.z);
        if (block != EMPTY && block != WATER && block != LAVA) {
            return true;
        }
    }

    return false;
}

void MyGL::initializeGL()
{

    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    // Enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();



    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of Cube
    mp_geomCube->create();
    mp_worldAxes->create();
    mp_crosshairs->create();
    // Draw the quad overlay
    m_geomQuad.create();
    m_inventory.create();
    m_selection.create();
    mp_SkyQuad->create();

    m_0->create();
    m_1->create();
    m_2->create();
    m_3->create();
    m_4->create();
    m_5->create();
    m_6->create();
    m_7->create();
    m_8->create();

    // Create and set up the diffuse shader
    mp_progLambert->create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    mp_progFlat->create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    mp_progMenu->create(":/glsl/inventory.vert.glsl", ":/glsl/inventory.frag.glsl");

    mp_progSky->create(":/glsl/sky.vert.glsl", ":/glsl/sky.frag.glsl");
    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    //mp_progLambert->setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    //    vao.bind();
    glBindVertexArray(vao);

    mp_terrain->CreateTestScene(this);
}

void MyGL::resizeGL(int w, int h)
{
    Camera* cam = new Camera(w, h, glm::vec3(mp_terrain->dimensions.x, mp_terrain->dimensions.y / 2, mp_terrain->dimensions.z),
                             glm::vec3(mp_terrain->dimensions.x/2, mp_terrain->dimensions.y/2, mp_terrain->dimensions.z /2), glm::vec3(0,1,0));

    cam->eye = glm::vec3(cam->eye.x - 20, cam->eye.y + 100, cam->eye.z - 20);
    mp_player->setCamera(cam); //set player's camera to this default
    mp_player->getCamera()->eye.x = 12;
    mp_player->getCamera()->eye.z = 12;
    mp_player->setPosition(cam->eye);

    //set player's ground

    mp_player->setGroundBlock((mp_terrain->getBlockAt(mp_player->getCamera()->eye.x, mp_player->getCamera()->eye.y - 3, mp_player->getCamera()->eye.z)));
    glm::mat4 viewproj = cam->getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    mp_progLambert->setViewProjMatrix(viewproj);
    mp_progFlat->setViewProjMatrix(viewproj);
    mp_progSky->setViewProjMatrix(glm::inverse(viewproj));

    //sky attr

    mp_progSky->useMe();
    this->glUniform2i(mp_progSky->unifDimensions, width() * this->devicePixelRatio(), height() * this->devicePixelRatio()); //pass u_Dimensions
    this->glUniform3f(mp_progSky->unifEyePos, mp_player->getCamera()->eye.x,  mp_player->getCamera()->eye.y,  mp_player->getCamera()->eye.z); //pass

    printGLErrorLog();
}


// MyGL's constructor links timerUpdate() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to use timerUpdate
void MyGL::timerUpdate()
{
    // Iterate through chunks to be added and append to terrain
    mp_progLambert->setTime(time);
    mp_progLambert->setEyePos(glm::vec4(mp_player->getCamera()->eye, 1.f));
    update();
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL()
{

    time = QDateTime::currentMSecsSinceEpoch() - time;
    mp_player->setPosition(mp_player->getCamera()->eye);
    mp_player->setDTime(time);

    if(!creativeMode) {

        bool collided = rayMarchCollision();

        if(collided == false) {
            mp_player->move();
            //std::cout<<" not collided so moving "<<std::endl;
        }
        bool collidedTop = rayMarchCollidedJump();
        if(collidedTop == false) {
            mp_player->jump();
            //std::cout<<" not collided on jump "<<std::endl;
        }

        mp_player->reduceVelocity();
        mp_player->checkGrounded();
        mp_player->setGroundBlock((mp_terrain->getBlockAt(mp_player->getCamera()->eye.x, mp_player->getCamera()->eye.y - 2.5f, mp_player->getCamera()->eye.z)));



        // See if players feet are touching a water block
        glm::vec3 pos = mp_player->getPosition();
        glm::vec3 feet = glm::vec3(pos.x, pos.y - 2.f, pos.z);
        BlockType atFeet = mp_terrain->getBlockAt(feet.x, feet.y, feet.z);
        if(atFeet == WATER || atFeet == LAVA) {
            mp_player->swimming = true;
            this->swimming = true;
        }
        else {
            mp_player->swimming = false;
            this->swimming = true;
        }
    }


    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glm:: mat4 viewproj = mp_player->getCamera()->getViewProj();
    mp_progFlat->setViewProjMatrix(viewproj);
    mp_progLambert->setViewProjMatrix(viewproj);
    mp_progSky->setViewProjMatrix(glm::inverse(viewproj));

    //sky

    mp_progSky->useMe();
    this->glUniform3f(mp_progSky->unifEyePos, mp_player->getCamera()->eye.x, mp_player->getCamera()->eye.y, mp_player->getCamera()->eye.z);
    this->glUniform1f(mp_progSky->unifTime, timeShader++);

    mp_progSky->draw(*mp_SkyQuad);

    GLDrawScene();

    if(!inventoryUp) {
        glDisable(GL_DEPTH_TEST);
        glLineWidth(1.f);
        mp_progFlat->setModelMatrix(glm::mat4());
        mp_progFlat->draw(*mp_worldAxes);
        mp_progFlat->setViewProjMatrix(glm::mat4());
        mp_progFlat->setModelMatrix(glm::mat4());
        mp_progFlat->draw(*mp_crosshairs);
        glEnable(GL_DEPTH_TEST);
    }

    MoveMouseToCenter();
}

void MyGL::GLDrawScene()
{
    QPair<int, int> chunkOrig = mp_player->originChunkBlockIn();
    glm::vec2 playerXZ = glm::vec2(mp_player->getPosition().x, mp_player->getPosition().z);
    glm::vec2 topLeftCorner = glm::vec2(chunkOrig.first, chunkOrig.second);
    glm::vec2 topRightCorner = glm::vec2(chunkOrig.first + 64, chunkOrig.second);
    glm::vec2 bottomLeftCorner = glm::vec2(chunkOrig.first, chunkOrig.second + 64);
    glm::vec2 bottomRightCorner = glm::vec2(chunkOrig.first + 64, chunkOrig.second + 64);

    QMutex mutex0;
    QMutex mutex;


    // Here, check if we need to add chunks based on camera pos
    // Check corners first
    if(glm::distance(playerXZ, topLeftCorner) < 5 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second - 64))) {
        // generate to top left
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 4, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }
    if(glm::distance(playerXZ, topRightCorner) < 5 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second - 64))) {
        // generate to top right
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 5, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }
    if(glm::distance(playerXZ, bottomLeftCorner) < 5 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second + 64))) {
        // generate to bottom left
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 6, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }
    if(glm::distance(playerXZ, bottomRightCorner) < 5 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second + 64))) {
        // generate to bottom right
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 7, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->chunks.value(chunkOrig)->create();mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }


    // Check direct horizontal and vertical positions
    if(mp_player->getPosition().x > (chunkOrig.first + 64) - 4 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second))) {
        // generate on right side
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 1, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }
    if(mp_player->getPosition().x < (chunkOrig.first) + 4 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second))) {
        // generate on left side
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 0, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }
    if(mp_player->getPosition().z > (chunkOrig.second + 64) - 4 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first, chunkOrig.second + 64))) {
        // generate on top side
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 3, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }
    if(mp_player->getPosition().z < (chunkOrig.second) + 4 && !mp_terrain->chunks.contains(QPair<int, int>(chunkOrig.first, chunkOrig.second - 64))) {
        // generate on bottom side
        std::vector<Chunk*> newChunks = mp_terrain->createChunks(this, 2, mp_player->getCamera()->eye);
        // Make a QRunnable for each chunk
        for(Chunk* newChunk : newChunks) {
            Blocker* newBlocker = new Blocker(&mutex0, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newBlocker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            Chunker* newChunker = new Chunker(&mutex, mp_terrain, newChunk);
            QThreadPool::globalInstance()->start(newChunker);
        }
        QThreadPool::globalInstance()->waitForDone();
        for(Chunk* newChunk : newChunks) {
            newChunk->createMembers();
        }
        mp_terrain->updateChunks(chunkOrig.first, chunkOrig.second);
    }

    bindAppropriateTexture();

    QList<Chunk*> chunks = mp_terrain->chunks.values();
    for(Chunk* chunk : chunks) {
        QPair<int, int> origin = chunk->origin;
        mp_progLambert->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(origin.first, 0, origin.second)));
        mp_progLambert->setEyePos(glm::vec4(mp_player->getCamera()->eye, 1.f));
        mp_progLambert->drawOpaque = true;
        mp_progLambert->draw(*chunk);
    }
    for(Chunk* chunk : chunks) {
        QPair<int, int> origin = chunk->origin;
        mp_progLambert->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(origin.first, 0, origin.second)));
        mp_progLambert->setEyePos(glm::vec4(mp_player->getCamera()->eye, 1.f));
        mp_progLambert->drawOpaque = false;
        mp_progLambert->draw(*chunk);
    }


    // Draw the QUAD overlay...if the camera is in water or lava!
    glm::vec3 position = mp_player->getCamera()->eye;
    BlockType atEye = mp_terrain->getBlockAt(position.x, position.y, position.z);
    if(atEye == WATER || atEye == LAVA) {
        mp_progFlat->setViewProjMatrix(glm::mat4());
        mp_progFlat->setModelMatrix(glm::mat4());

        if(atEye == WATER) {
            m_geomQuad.create();
            m_geomQuad.type = true;
        }
        else {
            m_geomQuad.create();
            m_geomQuad.type = false;
        }

        mp_progFlat->draw(m_geomQuad);
    }

    if(inventoryUp) {
        mp_progMenu->setViewProjMatrix(glm::mat4());
        mp_progMenu->setModelMatrix(glm::mat4());
        mp_progMenu->draw(m_inventory);

        glDisable(GL_DEPTH_TEST);
        mp_progFlat->setViewProjMatrix(glm::mat4());
        mp_progFlat->setModelMatrix(glm::mat4());
        mp_progFlat->draw(m_selection);

        mp_progMenu->setViewProjMatrix(glm::mat4());
        mp_progMenu->setModelMatrix(glm::mat4());
        mp_progMenu->draw(*m_0);
        mp_progMenu->draw(*m_1);
        mp_progMenu->draw(*m_2);
        mp_progMenu->draw(*m_3);
        mp_progMenu->draw(*m_4);
        mp_progMenu->draw(*m_5);
        mp_progMenu->draw(*m_6);
        mp_progMenu->draw(*m_7);
        mp_progMenu->draw(*m_8);

        glEnable(GL_DEPTH_TEST);


    }
}

void MyGL::bindAppropriateTexture()

{

    QImage img(":/minecraft_textures_all/minecraft_textures_all.png");
    img.convertToFormat(QImage::Format_RGBA8888);
    img = img.mirrored();
    std::shared_ptr<QImage> m_textureImage = std::make_shared<QImage>(img);
    glGenTextures(1, &m_textureHandle);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureHandle);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 m_textureImage->width(), m_textureImage->height(),
                 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_textureImage->bits());


    printGLErrorLog();

}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    if(creativeMode) {
        float amount = 2.0f;
        if(e->modifiers() & Qt::ShiftModifier){
            amount = 10.0f;
        }
        // http://doc.qt.io/qt-5/qt.html#Key-enum
        // This could all be much more efficient if a switch
        // statement were used, but I really dislike their
        // syntax so I chose to be lazy and use a long
        // chain of if statements instead
        if (e->key() == Qt::Key_Escape) {
            QApplication::quit();
        } else if (e->key() == Qt::Key_F) {
            creativeMode = !creativeMode;
            // Also turn on collisions
        } else if (e->key() == Qt::Key_I) {
            inventoryUp = !inventoryUp;
        }

        if(!inventoryUp) {
            if (e->key() == Qt::Key_Right) {
                mp_player->getCamera()->RotateAboutUp(-amount);
            } else if (e->key() == Qt::Key_Left) {
                mp_player->getCamera()->RotateAboutUp(amount);
            } else if (e->key() == Qt::Key_Up) {
                mp_player->getCamera()->RotateAboutRight(amount);
            } else if (e->key() == Qt::Key_Down) {
                mp_player->getCamera()->RotateAboutRight(-amount);
            }
        }
        else {
            if (e->key() == Qt::Key_Right) {
                switch(currBlock) {
                case GRASS:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = STONE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                }
            } else if (e->key() == Qt::Key_Left) {
                switch(currBlock) {
                case GRASS:
                    currBlock = STONE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                }
            } else if (e->key() == Qt::Key_Up) {
                switch(currBlock) {
                case GRASS:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = STONE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                }
            } else if (e->key() == Qt::Key_Down) {
                switch(currBlock) {
                case GRASS:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = ICE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                }
            }
        }

        if (e->key() == Qt::Key_1) {
            mp_player->getCamera()->fovy += amount;
        } else if (e->key() == Qt::Key_2) {
            mp_player->getCamera()->fovy -= amount;
        } else if (e->key() == Qt::Key_W) {
            mp_player->getCamera()->TranslateAlongLook(amount);
        } else if (e->key() == Qt::Key_S) {
            mp_player->getCamera()->TranslateAlongLook(-amount);
        } else if (e->key() == Qt::Key_D) {
            mp_player->getCamera()->TranslateAlongRight(amount);
        } else if (e->key() == Qt::Key_A) {
            mp_player->getCamera()->TranslateAlongRight(-amount);
        } else if (e->key() == Qt::Key_Q) {
            mp_player->getCamera()->TranslateAlongWorldUp(-amount);
        } else if (e->key() == Qt::Key_E) {
            mp_player->getCamera()->TranslateAlongWorldUp(amount);
        } else if (e->key() == Qt::Key_R) {
            *mp_player->getCamera() = Camera(this->width(), this->height());
        }
        mp_player->getCamera()->RecomputeAttributes();
    }
    else {
        if (e->key() == Qt::Key_F) {
            creativeMode = !creativeMode;
            // Also turn off collisions
        }
        else if (e->key() == Qt::Key_I) {
            inventoryUp = !inventoryUp;
        }
        if(inventoryUp) {
            if (e->key() == Qt::Key_Right) {
                switch(currBlock) {
                case GRASS:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = STONE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                }
            } else if (e->key() == Qt::Key_Left) {
                switch(currBlock) {
                case GRASS:
                    currBlock = STONE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                }
            } else if (e->key() == Qt::Key_Up) {
                switch(currBlock) {
                case GRASS:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = STONE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                }
            } else if (e->key() == Qt::Key_Down) {
                switch(currBlock) {
                case GRASS:
                    currBlock = LAVA;
                    m_selection.type = 3;
                    m_selection.create();
                    break;
                case DIRT:
                    currBlock = WOOD;
                    m_selection.type = 4;
                    m_selection.create();
                    break;
                case STONE:
                    currBlock = LEAF;
                    m_selection.type = 5;
                    m_selection.create();
                    break;
                case LAVA:
                    currBlock = BEDROCK;
                    m_selection.type = 6;
                    m_selection.create();
                    break;
                case WOOD:
                    currBlock = WATER;
                    m_selection.type = 7;
                    m_selection.create();
                    break;
                case LEAF:
                    currBlock = ICE;
                    m_selection.type = 8;
                    m_selection.create();
                    break;
                case BEDROCK:
                    currBlock = GRASS;
                    m_selection.type = 0;
                    m_selection.create();
                    break;
                case WATER:
                    currBlock = DIRT;
                    m_selection.type = 1;
                    m_selection.create();
                    break;
                case ICE:
                    currBlock = ICE;
                    m_selection.type = 2;
                    m_selection.create();
                    break;
                }
            }
        }
        mp_player->keyPressed(e);
    }
}

void MyGL::keyReleaseEvent(QKeyEvent *e)
{
    mp_player->keyReleased(e);
}

float MyGL::rayBoxInt(glm::vec3 box) {

    glm::vec3 r0 = mp_player->getCamera()->eye;
    glm::vec3 rd = mp_player->getCamera()->look;
    float x = box.x;
    float y = box.y;
    float z = box.z;

    float tNear = -std::numeric_limits<float>::max();
    float tFar = std::numeric_limits<float>::max();

    // For the X "slab":

    float t0x = (x - r0.x) / rd.x;
    float t1x = (x + 1.f - r0.x) / rd.x;

    if (t0x > t1x) std::swap(t0x, t1x);

    if (t0x > tNear) {
        tNear = t0x;
    }
    if (t1x < tFar) {
        tFar = t1x;
    }

    // For the Y "slab":
    float t0y = (y - r0.y) / rd.y;
    float t1y = (y + 1.f - r0.y) / rd.y;

    if (t0y > t1y) std::swap(t0y, t1y);

    if (t0y > tNear) {
        tNear = t0y;
    }
    if (t1y < tFar) {
        tFar = t1y;
    }

    // For the Z "slab":
    float t0z = (z - r0.z) / rd.z;
    float t1z = (z + 1.f - r0.z) / rd.z;

    if (t0z > t1z) std::swap(t0z, t1z);

    if (t0z > tNear) {
        tNear = t0z;
    }
    if (t1z < tFar) {
        tFar = t1z;
    }

    return glm::min(tNear, tFar);
}

glm::vec3 MyGL::rayMarching(float nearBox, float farBox) {
    glm::vec3 r0 = mp_player->getCamera()->eye;
    glm::vec3 rd = mp_player->getCamera()->look;
    for (float t = nearBox; t < farBox; t += 0.01f) {
        glm::vec3 pos = r0 + t * rd;
        pos = glm::vec3((int) pos.x, (int) pos.y, (int) pos.z);
        if (mp_terrain->getBlockAt(pos.x, pos.y, pos.z) != EMPTY) {
            return pos;
        }
    }
    return glm::vec3(0,0,0);
}

void MyGL::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        glm::vec3 box = rayMarching(0.1f, 1.5f);
        if (box != glm::vec3(0,0,0)) {
            // Add deleted block to inventory
            BlockType delBlock = mp_terrain->getBlockAt(box.x, box.y, box.z);
            switch(delBlock) {
            case GRASS:
                m_0->number++;
                if(m_0->number > 9) {
                    m_0->number = 9;
                }
                m_0->create();
                break;
            case DIRT:
                m_1->number++;
                if(m_1->number > 9) {
                    m_1->number = 9;
                }
                m_1->create();
                break;
            case STONE:
                m_2->number++;
                if(m_2->number > 9) {
                    m_2->number = 9;
                }
                m_2->create();
                break;
            case LAVA:
                m_3->number++;
                if(m_3->number > 9) {
                    m_3->number = 9;
                }
                m_3->create();
                break;
            case WOOD:
                m_4->number++;
                if(m_4->number > 9) {
                    m_4->number = 9;
                }
                m_4->create();
                break;
            case LEAF:
                m_5->number++;
                if(m_5->number > 9) {
                    m_5->number = 9;
                }
                m_5->create();
                break;
            case BEDROCK:
                m_6->number++;
                if(m_6->number > 9) {
                    m_6->number = 9;
                }
                m_6->create();
                break;
            case WATER:
                m_7->number++;
                if(m_7->number > 9) {
                    m_7->number = 9;
                }
                m_7->create();
                break;
            case ICE:
                m_8->number++;
                if(m_8->number > 9) {
                    m_8->number = 9;
                }
                m_8->create();
                break;
            }

            mp_terrain->setBlockAt(box.x, box.y, box.z, EMPTY);
            // Update current chunk
            int origX;
            int origZ;
            if(box.x < 0) {
                origX = (((int) box.x / 16) * 16);
                if((int) box.x % 16 != 0) {
                    origX -= 16;
                }
            }
            else {
                origX = ((int) box.x / 16) * 16;
            }
            if(box.z < 0) {
                origZ = (((int) box.z / 16) * 16);
                if((int) box.z % 16 != 0) {
                    origZ -= 16;
                }
            }
            else {
                origZ = ((int) box.z / 16) * 16;
            }
            QPair<int, int> origin = QPair<int, int>(origX, origZ);
            mp_terrain->chunks.value(origin)->create();
        }

    } else if(e->buttons() & Qt::RightButton)
    {
        glm::vec3 box = rayMarching(2.f, 3.5f);
        if (box != glm::vec3(0,0,0)) {
            float t = rayBoxInt(box);
            glm::vec3 pos = mp_player->getCamera()->eye + t * mp_player->getCamera()->look;

            glm::vec3 midBox = box + 0.5f;

            glm::vec3 norm;

            float xDist = glm::abs(midBox.x - pos.x);
            float yDist = glm::abs(midBox.y - pos.y);
            float zDist = glm::abs(midBox.z - pos.z);

            if (xDist >= yDist && xDist >= zDist) {
                norm = glm::vec3(glm::sign(pos.x - midBox.x), 0.f, 0.f);
            } else if (yDist > xDist && yDist > zDist) {
                norm = glm::vec3(0.f, glm::sign(pos.y - midBox.y), 0.f);
            } else {
                norm = glm::vec3(0.f, 0.f, glm::sign(pos.z - midBox.z));
            }

            glm::vec3 newBox = box + norm;
            newBox = glm::vec3((int) newBox.x, (int) newBox.y, (int) newBox.z);
            if (mp_terrain->getBlockAt(newBox.x, newBox.y, newBox.z) == EMPTY) {

                switch(currBlock) {
                case GRASS:
                    if(m_0->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_0->number--;
                        if(m_0->number < 0) {
                            m_0->number = 0;
                        }

                        m_0->create();
                    }
                    break;
                case DIRT:
                    if(m_1->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_1->number--;
                        if(m_1->number < 0) {
                            m_1->number = 0;
                        }
                        m_1->create();
                    }
                    break;
                case STONE:
                    if(m_2->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_2->number--;
                        if(m_2->number < 0) {
                            m_2->number = 0;
                        }
                        m_2->create();
                    }
                    break;
                case LAVA:
                    if(m_3->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_3->number--;
                        if(m_3->number < 0) {
                            m_3->number = 0;
                        }
                        m_3->create();
                    }
                    break;
                case WOOD:
                    if(m_4->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_4->number--;
                        if(m_4->number < 0) {
                            m_4->number = 0;
                        }
                        m_4->create();
                    }
                    break;
                case LEAF:
                    if(m_5->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_5->number--;
                        if(m_5->number < 0) {
                            m_5->number = 0;
                        }
                        m_5->create();
                    }
                    break;
                case BEDROCK:
                    if(m_6->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_6->number--;
                        if(m_6->number < 0) {
                            m_6->number = 0;
                        }
                        m_6->create();
                    }
                    break;
                case WATER:
                    if(m_7->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_7->number--;
                        if(m_7->number < 0) {
                            m_7->number = 0;
                        }
                        m_7->create();
                    }
                    break;
                case ICE:
                    if(m_8->number != 0) {
                        mp_terrain->setBlockAt(newBox.x, newBox.y, newBox.z, currBlock);
                        m_8->number--;
                        if(m_0->number < 0) {
                            m_0->number = 0;
                        }
                        m_8->create();
                    }
                    break;
                }
                // Update current chunk
                int origX;
                int origZ;
                if(newBox.x < 0) {
                    origX = (((int) newBox.x / 16) * 16);
                    if((int) newBox.x % 16 != 0) {
                        origX -= 16;
                    }
                }
                else {
                    origX = ((int) newBox.x / 16) * 16;
                }
                if(newBox.z < 0) {
                    origZ = (((int) newBox.z / 16) * 16);
                    if((int) newBox.z % 16 != 0) {
                        origZ -= 16;
                    }
                }
                else {
                    origZ = ((int) newBox.z / 16) * 16;
                }
                QPair<int, int> origin = QPair<int, int>(origX, origZ);
                mp_terrain->chunks.value(origin)->create();
            }
        }
    }
}

void MyGL::mouseMoveEvent(QMouseEvent *e) {
    //this->MoveMouseToCenter();
    mp_player->mousePressed(e);
}

void MyGL::mouseReleaseEvent(QMouseEvent *e) {
    mp_player->mousePressed(e);
}
