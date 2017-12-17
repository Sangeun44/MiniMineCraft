#include "player.h"

Player::Player() :
    pos(), camera(new Camera()), velocity(glm::vec3(0,0,0)), grounded(false), swimming(false),
    block(), w(false), a(false), s(false), d(false), space(false), left(false), right(false),
    cursor_x(), cursor_y(), dT()
{
    pos = camera->eye;
}

//destructor
Player::~Player() {}

void Player::setPosition(const glm::vec3 pos2) {
    camera->eye = pos2;
    pos = pos2;
}


void Player::setCamera(Camera* c) {
    camera = c;
}

void Player::setDTime(const float t) {
    dT = t;
}

void Player::setGroundBlock(const BlockType e) {
    block = e;
}

Camera* Player::getCamera() const {
    return camera;
}

glm::vec3 Player::getPosition() const{
    return pos;
}

float Player::getCursorX() const{
    return cursor_x;
}

float Player::getCursorY() const {
    return cursor_y;
}

QString Player::getKey() const{

    if(w == true) {
        return "w";
    }

    else if(a == true) {
        return "a";
    }

    else if(s == true) {
        return "s";
    }

    else if(d == true) {
        return "d";
    }
}

QString Player::getButton() const{
    if(left == true) {
        return "left";
    }
    else if(right == true) {
        return "right";
    }
}

glm::vec3 Player::getVelocity() const {
    return velocity;
}

bool Player::getJump() const{
    return space;
}

//check the ground block beneath
void Player::checkGrounded() {
    if(camera->eye.y - 2 < 0) {
        grounded = false;
        velocity.y = -10.f;
    }
    if(block == EMPTY || block == WATER || block == LAVA) {
        grounded = false;
        velocity.y -= .5f;
    }
    else {
        grounded = true;
        velocity.y = 0.f;
    }
}

void Player::jump() {
    if(velocity.y > 0) {
        move();
    }
}

//move camera if the velocity is not static// update position
void Player::move() {
    if(velocity.z != 0) {
        glm::vec3 grounded = glm::normalize(glm::vec3(camera->look.x, 0, camera->look.z));
        camera->eye += grounded * (velocity.z * dT/1000000000000);
        camera->ref += camera->look;
    }
    if(velocity.x != 0) {
        camera->TranslateAlongRight(velocity.x * dT/1000000000000);
    }
    if(velocity.y != 0) {
        glm::vec3 grounded = glm::normalize(glm::vec3(0, 2, 0));
        camera->eye += grounded * (velocity.y * dT/1000000000000);
        camera->ref += camera->look;
        pos = camera->eye;
    }
}

//friction and gravity update
void Player::reduceVelocity() {
        if(velocity.y > 0) {
            velocity.y -= 0.2f;
        }

        if(grounded = true) {

            if(swimming) {
                velocity.y = 0.436;
            }
            else {
                velocity.y = 0.f;
            }
        }

        if(grounded == false) {
            velocity.y -= .5f;
        }

        // Removed to implement smoother swimming
        /*
        if(velocity.z > 0) {
            //velocity.z -= .5f;//
        }

        else if(velocity.z < 0) {
            //velocity.z += .5f;
        }

        if(velocity.x > 0) {
            //velocity.x -= 0.5f;
        }

        else if(velocity.x < 0) {
            //velocity.x += 0.5f;
        }
        */
}

//key pressed
void Player::keyPressed(QKeyEvent *e) {
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_1) {
        camera->fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        camera->fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        velocity.z += .35f;
        if(swimming) {
            // Reduce velocity if swimming
            velocity.z -= .20;
        }
        w = true;
    } else if (e->key() == Qt::Key_S) {
        velocity.z -= .35f;
        if(swimming) {
            // Reduce velocity if swimming
            velocity.z += .20;
        }
        s = true;
    } else if (e->key() == Qt::Key_D) {
        velocity.x += .35f;
        if(swimming) {
            // Reduce velocity if swimming
            velocity.x -= .20;
        }
        d = true;
    } else if (e->key() == Qt::Key_A) {
        velocity.x -= .35f;
        if(swimming) {
            // Reduce velocity if swimming
            velocity.x += .20;
        }
        a = true;
    } else if(e->key() == Qt::Key_Space) {
      if(swimming) {
          camera->TranslateAlongWorldUp(.32f);
      }
      if(grounded == true)
       {
          grounded = false;
          velocity.y += 2.f;
          space = true;
      }
    }
    camera->RecomputeAttributes();
}

//key released
void Player::keyReleased(QKeyEvent *e) {
    if (e->key() == Qt::Key_W) {
        w = false;
        velocity.z = 0.f;
    } else if (e->key() == Qt::Key_S) {
        s = false;
        velocity.z = 0.f;
    } else if (e->key() == Qt::Key_D) {
        d = false;
        velocity.x = 0.f;
    } else if (e->key() == Qt::Key_A) {
        a = false;
        velocity.x = 0.f;
    } else if(e->key() == Qt::Key_Space) {
        space = false;    
    }
}

//mouse income
void Player::mousePressed(QMouseEvent *e) {
    float amount = 2.f;

    Qt::MouseButton button = e->button();
    cursor_x = (float)e->x();
    cursor_y = (float)e->y();

    if(button == Qt::LeftButton) {
        left = true;
    }
    if(button == Qt::RightButton) {
        right = true;
    }
    if(cursor_x < camera->width/2) {
        camera->RotateAboutUp(amount);
    }
    else if(cursor_x > camera->width/2){
        camera->RotateAboutUp(-amount);
    }
    if(cursor_y < camera->height/2) {
        camera->RotateAboutRight(amount);
    }
    else if(cursor_y > camera->height/2){
        camera->RotateAboutRight(-amount);
    }
}

QPair<int, int> Player::originChunkBlockIn() {
    QPair<int, int> origin = QPair<int, int>(((int) this->getPosition().x / 64) * 64, ((int) this->getPosition().z / 64) * 64);
    return origin;
}


