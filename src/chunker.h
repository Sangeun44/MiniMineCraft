#ifndef CHUNKER
#define CHUNKER

#include <QRunnable>
#include <QString>
#include <QMutex>
#include "scene/terrain.h"

class Chunker : public QRunnable {
private:
    QMutex* mutex;
    Terrain* terrain;
    Chunk* chunk;

public:
    Chunker(QMutex* mutex, Terrain* terrain, Chunk* chunk);
    void run() override;
};




#endif // CHUNKER

