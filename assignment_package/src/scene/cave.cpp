
#include "cave.h"

Cave::Cave() : turtle(new Turtle), axiom("FX"), width(10), distance(15)
{}

Cave::~Cave()
{}

void Cave::setTurtleIteration(const int it) {
    turtle->setCurrIteration(it);
}

void Cave::setTurtlePosition(const glm::vec3 pos) {
    turtle->setPosition(pos);
}

void Cave::setTurtleOrientation(const glm::vec3 orient) {
    turtle->setOrientation(orient);
}

glm::vec3 Cave::getTurtlePosition() const {
    return turtle->getPosition();
}

glm::vec3 Cave::getTurtleOrientation() const {
    return turtle->getOrientation();
}

int Cave::getTurtleIteration() const {
    return turtle->getIteration();
}

void Cave::setMapCharToFptr(const QChar c, Rule x) {
    mapCtoFptr.insert(c, x);
}

void Cave::setMapCharToString(const QChar c, const QString s) {
    mapCtoS.insert(c, s);
}

QHash<QChar, QString> Cave::getMapCharToString() const {
    return mapCtoS;
}

QHash<QChar, Rule> Cave::getMapCharToFptr() const {
    return mapCtoFptr;
}

QString Cave::getAxiom() const {
    return axiom;
}

Turtle* Cave::getTurtle() const {
    return turtle;
}

int Cave::getWidth() const {
    return width;
}
void Cave::setWidth(const int x) {
   width = x;
}

int Cave::getDistance() const {
    return distance;
}
void Cave::setDistance(const int x)  {
   distance = x;
}

void Cave::setBlocks(const glm::vec3 block) {
    caveBlocks.push_back(block);
}

std::vector<glm::vec3> Cave::getBlocks() const {
    return caveBlocks;
}
