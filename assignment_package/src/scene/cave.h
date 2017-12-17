#pragma once
#include <la.h>
#include <utils.h>
#include <scene/turtle.h>
#include <QHash>
#include "terrain.h"

class Terrain;
typedef void (Terrain::*Rule)(Turtle*,int); //function pointer


class Cave
{
public:
    Cave();
    ~Cave();

    //setters for turtle
    void setTurtlePosition(const glm::vec3 pos);
    void setTurtleOrientation(const glm::vec3 orient);
    void setTurtleIteration(const int it);
    void setDistance(const int i);
    void setWidth(const int i);
    void setBlocks(const glm::vec3);

    //getters for turtle
    glm::vec3 getTurtlePosition() const;
    glm::vec3 getTurtleOrientation() const;
    int getTurtleIteration() const;
    QString getAxiom() const;
    Turtle* getTurtle() const;
    int getWidth() const;
    int getDistance() const;
    std::vector<glm::vec3> getBlocks() const;

    //set map to char
    void setMapCharToString(const QChar c, const QString s);
    void setMapCharToFptr(const QChar c, Rule x);

    //get map to char
    QHash<QChar, QString> getMapCharToString() const;
    QHash<QChar, Rule> getMapCharToFptr() const;

private:
    Turtle* turtle;
    QString axiom;
    int width;
    int distance;
    std::vector<glm::vec3> caveBlocks; //store the blocks here
    QHash<QChar, QString> mapCtoS; //from char to String
    QHash<QChar, Rule> mapCtoFptr; //from char to function pointer

};
