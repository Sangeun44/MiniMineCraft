#pragma once

#include <la.h>
#include <camera.h>
#include <scene/terrain.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>

class Player
{
public:
    Player();
    ~Player();

    void keyPressed(QKeyEvent *e); //when a key is pressed in ui, send from mygl
    void keyReleased(QKeyEvent *e); //when a key is released in ui, send from mygl
    void mousePressed(QMouseEvent *e); //when mouse is pressed, released/dragged, sent from mygl

    //physics
    void reduceVelocity();
    void move();
    void jump();

    //getters
    Camera* getCamera() const;
    glm::vec3 getPosition() const;
    float getCursorX() const;
    float getCursorY() const;
    QString getKey() const;
    QString getButton() const;
    bool getJump() const;
    glm::vec3 getVelocity() const;

    //setters
    void setCamera(Camera* c);
    void setPosition(const glm::vec3 p);
    void setDTime(const float t);
    void setVelocity(const glm::vec3 v);
    void setGroundBlock(const BlockType e);

    //Terrain block
    void checkGrounded(); //check if the player has a ground beneath


    bool swimming;
    bool grounded; //on ground

    QPair<int, int> originChunkBlockIn(); // Checks what chunk the player is in based on player position

private:
    glm::vec3 pos; //position of player

    Camera* camera; // player pov

    glm::vec3 velocity; //velocity direction

   BlockType block;

    //key currently pressed
    bool w;
    bool a;
    bool s;
    bool d;
    bool space;

    //left/right mouse
    bool left;
    bool right;

    //cursor position
    float cursor_x;
    float cursor_y;

    //delta Time
    float dT;

};

