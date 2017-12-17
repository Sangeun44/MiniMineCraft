#include "blockers.h"

Blocker::Blocker(QMutex *mutex, Terrain *terrain, Chunk *chunk)
    : mutex(mutex), terrain(terrain), chunk(chunk)
{}

void Blocker::run() {
    mutex->lock();

    // set block types for chunk
    for(int x = chunk->origin.first; x < chunk->origin.first + 16; ++x)
    {
        for(int z = chunk->origin.second; z < chunk->origin.second + 16; ++z)
        {
            int r = (terrain->fractal(glm::vec2(x, z) / 64.f)) * 70 + 128;
            for(int y = 0; y < 256; ++y)
            {
                if(y <= 128)
                {
                    terrain->setBlockAt(x, y, z, STONE);
                }
                else if (y < r) {
                    terrain->setBlockAt(x, y, z, DIRT);
                } else if (y == r) {
                    terrain->setBlockAt(x, y, z, GRASS);
                } else {
                    terrain->setBlockAt(x, y, z, EMPTY);
                }
            }
        }
    }
    mutex->unlock();

    mutex->lock();
    // flood
    for(int x = chunk->origin.first; x < chunk->origin.first + 16; ++x) {
        for(int z = chunk->origin.second; z < chunk->origin.second + 16; ++z) {
            for(int y = 0; y < 160; ++y) {
                if(terrain->getBlockAt(x, y, z) == EMPTY) {
                    terrain->setBlockAt(x, y, z, WATER);
                }
            }
        }
    }

    //terrain->setRivers();
    mutex->unlock();
}



