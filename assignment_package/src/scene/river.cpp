#include "river.h"

River::River() : turtle(new Turtle), axiom("FX"), width(10), distance(15)
{}

River::~River()
{}

void River::setTurtleIteration(const int it) {
    turtle->setCurrIteration(it);
}

void River::setTurtlePosition(const glm::vec3 pos) {
    turtle->setPosition(pos);
}

void River::setTurtleOrientation(const glm::vec3 orient) {
    turtle->setOrientation(orient);
}

glm::vec3 River::getTurtlePosition() const {
    return turtle->getPosition();
}

glm::vec3 River::getTurtleOrientation() const {
    return turtle->getOrientation();
}

int River::getTurtleIteration() const {
    return turtle->getIteration();
}

void River::setMapCharToFptr(const QChar c, Rule x) {
    mapCtoFptr.insert(c, x);
}

void River::setMapCharToString(const QChar c, const QString s) {
    mapCtoS.insert(c, s);
}

QHash<QChar, QString> River::getMapCharToString() const {
    return mapCtoS;
}

QHash<QChar, Rule> River::getMapCharToFptr() const {
    return mapCtoFptr;
}

QString River::getAxiom() const {
    return axiom;
}

Turtle* River::getTurtle() const {
    return turtle;
}

int River::getWidth() const {
    return width;
}
void River::setWidth(const int x) {
   width = x;
}

int River::getDistance() const {
    return distance;
}
void River::setDistance(const int x)  {
   distance = x;
}

void River::setBlocks(const glm::vec3 block) {
    riverBlocks.push_back(block);
}

std::vector<glm::vec3> River::getBlocks() const {
    return riverBlocks;
}
