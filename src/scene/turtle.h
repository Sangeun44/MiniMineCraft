#ifndef TURTLE_H
#define TURTLE_H
#include <la.h>

class Turtle
{
public:
    Turtle();
    ~Turtle();

    //setters
    void setPosition(const glm::vec3 x);
    void setOrientation(const glm::vec3 x);
    void setCurrIteration(const int x);

    //getters
    glm::vec3 getPosition() const;
    glm::vec3 getOrientation() const;
    int getIteration() const;

private:
    glm::vec3 position;
    glm::vec3 orientation;
    int maxDepth;
    int iteration;
};


#endif // TURTLE_H
