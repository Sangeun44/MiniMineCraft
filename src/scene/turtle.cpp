#include "turtle.h"

Turtle::Turtle() : position(glm::vec3(0,140,20)), orientation(glm::vec3(0,0,1)), maxDepth(2), iteration(3)
{
}

Turtle::~Turtle() {}

//setters
void Turtle::setPosition(const glm::vec3 x) {
    position = x;
}

void Turtle::setOrientation(const glm::vec3 x) {
    orientation = x;
}

void Turtle::setCurrIteration(const int x) {
   iteration = x;
}

//getters
glm::vec3 Turtle::getPosition() const {
    return position;
}

glm::vec3 Turtle::getOrientation() const {
    return orientation;
}

int Turtle::getIteration() const {
    return iteration;
}
