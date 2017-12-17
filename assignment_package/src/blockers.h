#ifndef BLOCKERS
#define BLOCKERS

#include <QRunnable>
#include <QString>
#include <QMutex>
#include "scene/terrain.h"

class Blocker : public QRunnable {
private:
    QMutex* mutex;
    Terrain* terrain;
    Chunk* chunk;

public:
    Blocker(QMutex* mutex, Terrain* terrain, Chunk* chunk);
    void run() override;
};

#endif // BLOCKERS

