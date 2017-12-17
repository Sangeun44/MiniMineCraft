#include <scene/terrain.h>
#include "mygl.h"
#include <scene/cube.h>
#include <glm/gtx/rotate_vector.hpp>

Terrain::Terrain() : chunks(QHash<QPair<int, int>, Chunk*>()), dimensions(64, 256, 64), worldBounds(0, 64, 0, 64)
{}

BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    int origX;
    int origZ;
    if(x < 0) {
        origX = ((x / 16) * 16);
        if(x % 16 != 0) {
            origX -= 16;
        }
    }
    else {
        origX = (x / 16) * 16;
    }
    if(z < 0) {
        origZ = ((z / 16) * 16);
        if(z % 16 != 0) {
            origZ -= 16;
        }
    }
    else {
        origZ = (z / 16) * 16;
    }
    Chunk* chunk = chunks.value(QPair<int, int>(origX, origZ));
    int chunkX = x - (origX);
    int chunkZ = z - (origZ);
    return chunk->getBlockAt(chunkX, y, chunkZ);
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    int origX;
    int origZ;
    if(x < 0) {
        origX = ((x / 16) * 16);
        if(x % 16 != 0) {
            origX -= 16;
        }
    }
    else {
        origX = (x / 16) * 16;
    }
    if(z < 0) {
        origZ = ((z / 16) * 16);
        if(z % 16 != 0) {
            origZ -= 16;
        }
    }
    else {
        origZ = (z / 16) * 16;
    }

    Chunk* chunk = chunks.value(QPair<int, int>(origX, origZ));
    int chunkX = x - (origX);
    int chunkZ = z - (origZ);
    BlockType& block = chunk->setBlockAt(chunkX, y, chunkZ);
    block = t;
}

float rand(glm::vec2 n) {
    return glm::fract(sin(glm::dot(n, glm::vec2(12.9898, 4.1414))) * 43758.5453);
}

float makeNoise(glm::vec2 n) {
    glm::vec2 floor = glm::floor(n);
    glm::vec2 fract = glm::fract(n);
    float a = rand(floor);
    float b = rand(floor + glm::vec2(1.0, 0.0));
    float c = rand(floor + glm::vec2(0.0, 1.0));
    float d = rand(floor + glm::vec2(1.0, 1.0));
    glm::vec2 fractVec = fract * fract * (3.f - 2.f * fract);

    return glm::mix(a, b, fractVec.x) + (c - a) * fractVec.y * (1.f - fractVec.x) +
            (d - b) * fractVec.x * fractVec.y;
}

float Terrain::fractal(glm::vec2 n) {
    float value = 0.0;
    float amp = .5;
    glm::vec2 shift = glm::vec2(100.0);
    glm::mat2 rot = glm::mat2(cos(0.5), sin(0.5),
                              -sin(0.5), cos(0.50));

    for (int i = 0; i < 6; i++) {
        value += amp * makeNoise(n);
        n = rot * n * 2.f + shift;
        amp*= .5;
    }
    return value;
}

void Terrain::CreateTestScene(OpenGLContext* context)
{
    std::vector<Chunk*> testChunks;

<<<<<<< HEAD
    int x_bound = 64;
    int y_bound = 256;
    int z_bound = 64;
=======
        int x_bound = 64;
        int y_bound = 256;
        int z_bound = 64;
>>>>>>> 715fc8b33e239703627d70ec592b2ed32d57e4b9

    for(int x = 0; x < x_bound; x += 16) {
        for(int z = 0; z < z_bound; z += 16) {
            Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
            testChunks.push_back(newChunk);
            chunks.insert(QPair<int, int>(x, z), newChunk);
        }
    }

    //Initialize each block to empty
    for(int x = 0; x < x_bound; ++x) {
        for(int z = 0; z < z_bound; ++z) {
            for(int y = 0; y_bound < 256; y++) {
                setBlockAt(x, y, z, EMPTY);
            }
        }
    }

    for(int x = 0; x < x_bound; ++x) {
        for(int z = 0; z < z_bound; ++z) {
            int r = (fractal(glm::vec2(x, z) / 64.f)) * 70 + 128;
            for(int y = 0; y < y_bound; ++y) {
                if(y <= 128) {
                    setBlockAt(x, y, z, STONE);
                } else if (y < r) {
                    setBlockAt(x, y, z, DIRT);
                } else if (y == r) {
                    setBlockAt(x, y, z, GRASS);
                } else {
                    setBlockAt(x, y, z, EMPTY);
                }
            }
        }
    }

<<<<<<< HEAD

    setRivers(context);

    if(!rivers.isEmpty()) {
        createBlocksForRiver(context);
    }

    setCaves(context);

    if(!caves.isEmpty()) {
        createEntrance();
        createBlocksForCaves(context);
    }


    // flood terrain
    for(int x = 0; x < x_bound; ++x) {
        for(int z = 0; z < z_bound; ++z) {
            for(int y = 0; y < 117; ++y) {
                if(getBlockAt(x, y, z) == EMPTY) {
                    setBlockAt(x, y, z, LAVA);
=======
       
//        setRivers(context);

//        if(!rivers.isEmpty()) {
//            createChunksForRiver(context);
//            createBlocksForRiver(context);
//        }

//        setCaves(context);

//        if(!caves.isEmpty()) {
//            createEntrance();
//            createChunksForCaves(context);
//            createBlocksForCaves(context);
//        }
        

        // flood terrain
        for(int x = 0; x < x_bound; ++x) {
            for(int z = 0; z < z_bound; ++z) {
                for(int y = 0; y < 117; ++y) {
                    if(getBlockAt(x, y, z) == EMPTY) {
                        setBlockAt(x, y, z, LAVA);
                    }
>>>>>>> 715fc8b33e239703627d70ec592b2ed32d57e4b9
                }
            }
        }
    }

    for(Chunk* chunk : chunks) {
        chunk->create();
    }
}

void Terrain::createBlocksForRiver(OpenGLContext* context) {
    for(River* r : rivers) {
        std::vector<glm::vec3> riverBlocks = r->getBlocks();
        for(glm::vec3 curr: riverBlocks) {
            int x = curr.x;
            int y = curr.y;
            int z = curr.z;
            if(x > worldBounds[0] && z > worldBounds[2] && x < worldBounds[1] && z < worldBounds[3] && y > 0 && y < 256) {

                int i = 0;
                setBlockAt(x, y , z , WATER);
                while(curr.y + i < 256) {
                    if(getBlockAt(x, y + i, z) != WATER) {
                        setBlockAt(x, y + i, z, EMPTY);
                    }
                    i++;
                }
                for(int i = 0; i < 140; ++i) {
                    if(getBlockAt(x, i, z) == EMPTY) {
                        setBlockAt(x, i, z, WATER);
                    }
                }
                for(int i = 141; i < 256; i++) {
                    for(int j = -3; j < 3; j++) {
                        glm::vec3 pos1 = glm::vec3(x, i, z + j);
                        if(pos1.x > worldBounds[0] && pos1.z > worldBounds[2] && pos1.x < worldBounds[1] && pos1.z < worldBounds[3]) {
                            BlockType block1 = getBlockAt(pos1.x, i, pos1.z);
                            if(block1 != WATER && block1 != EMPTY) {
                                setBlockAt(pos1.x, i, pos1.z, EMPTY);
                            }
                        }
                    }
                }
                for(int i = 145; i < 256; i++) {
                    for(int j = -5; j < 5; j++) {
                        glm::vec3 pos1 = glm::vec3(curr.x, i, curr.z + j);
                        if(pos1.x > worldBounds[0] && pos1.z > worldBounds[2] && pos1.x < worldBounds[1] && pos1.z < worldBounds[3]) {
                            BlockType block1 = getBlockAt(pos1.x, i, pos1.z);
                            if(block1 != WATER && block1 != EMPTY) {
                                setBlockAt(pos1.x, i, pos1.z, EMPTY);
                            }
                        }
                    }
                }
                for(int i = 150; i < 256; i++) {
                    for(int j = -10; j < 10; j++) {
                        glm::vec3 pos1 = glm::vec3(curr.x, i, curr.z + j);
                        if(pos1.x > worldBounds[0] && pos1.z > worldBounds[2] && pos1.x < worldBounds[1] && pos1.z < worldBounds[3]) {
                            BlockType block1 = getBlockAt(pos1.x, i, pos1.z);
                            if(block1 != WATER && block1 != EMPTY) {
                                setBlockAt(pos1.x, i, pos1.z, EMPTY);
                            }
                        }
                    }
                }
                for(int i = 155; i < 256; i++) {
                    for(int j = -20; j < 20; j++) {
                        glm::vec3 pos1 = glm::vec3(curr.x, i, curr.z + j);
                        if(pos1.x > worldBounds[0] && pos1.z > worldBounds[2] && pos1.x < worldBounds[1] && pos1.z < worldBounds[3]) {
                            BlockType block1 = getBlockAt(pos1.x, i, pos1.z);
                            if(block1 != WATER && block1 != EMPTY) {
                                setBlockAt(pos1.x, i, pos1.z, EMPTY);
                            }
                        }
                    }
                }
                for(int i = 160; i < 256; i++) {
                    for(int j = -25; j < 25; j++) {
                        glm::vec3 pos1 = glm::vec3(curr.x, i, curr.z + j);
                        if(pos1.x > worldBounds[0] && pos1.z > worldBounds[2] && pos1.x < worldBounds[1] && pos1.z < worldBounds[3]) {
                            BlockType block1 = getBlockAt(pos1.x, i, pos1.z);
                            if(block1 != WATER && block1 != EMPTY) {
                                setBlockAt(pos1.x, i, pos1.z, EMPTY);
                            }
                        }
                    }
                }
            }
        }
    }
}


//void Terrain::createChunksForRiver(OpenGLContext* context) {
//    QMutex mutex;

//    for(River* r : rivers) {

//        std::vector<glm::vec3> riverBlocks = r->getBlocks();
//        for(glm::vec3 curr : riverBlocks) {
//            int x = ((int)(curr.x/64)) * 64;
//            int z = ((int)(curr.z/64)) * 64;
//            QPair<int,int> chunkOrig = QPair<int,int>();
//            chunkOrig.first = x;
//            chunkOrig.second = z;

//            glm::vec2 blockXZ = glm::vec2(curr.x, curr.z);

//            glm::vec2 topLeftCorner = glm::vec2(chunkOrig.first, chunkOrig.second);
//            glm::vec2 topRightCorner = glm::vec2(chunkOrig.first + 64, chunkOrig.second);
//            glm::vec2 bottomLeftCorner = glm::vec2(chunkOrig.first, chunkOrig.second + 64);
//            glm::vec2 bottomRightCorner = glm::vec2(chunkOrig.first + 64, chunkOrig.second + 64);

//            if(glm::distance(blockXZ, topLeftCorner) < 60 && !chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second - 64))) {
//                // generate to top left
//                std::vector<Chunk*> newChunks = createChunks(context, 4, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(glm::distance(blockXZ, topRightCorner) < 60 && !chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second - 64))) {
//                // generate to top right
//                std::vector<Chunk*> newChunks = createChunks(context, 5, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(glm::distance(blockXZ, bottomLeftCorner) < 60 && !chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second + 64))) {
//                // generate to bottom left
//                std::vector<Chunk*> newChunks = createChunks(context, 6, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(glm::distance(blockXZ, bottomRightCorner) < 60 && !chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second + 64))) {
//                // generate to bottom right
//                std::vector<Chunk*> newChunks = createChunks(context, 7, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            // Check direct horizontal and vertical positions
//            if(curr.x > (chunkOrig.first + 64) - 60 && !chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second))) {
//                // generate on right side
//                std::vector<Chunk*> newChunks = createChunks(context, 1, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(curr.x < (chunkOrig.first) + 60 && !chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second))) {
//                // generate on left side
//                std::vector<Chunk*> newChunks = createChunks(context, 0, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(curr.z > (chunkOrig.second + 64) - 60 && !chunks.contains(QPair<int, int>(chunkOrig.first, chunkOrig.second + 64))) {
//                // generate on top side
//                std::vector<Chunk*> newChunks = createChunks(context, 3, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(curr.z < (chunkOrig.second) + 60 && !chunks.contains(QPair<int, int>(chunkOrig.first, chunkOrig.second - 64))) {
//                // generate on bottom side
//                std::vector<Chunk*> newChunks = createChunks(context, 2, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//        }
//    }
//}


//set rivers
void Terrain::setRivers(OpenGLContext* context) {
    River* newRiver1 = new River(); //delta river
    QString string1 = "S[-FX]+FX";
    QChar charToString1 = 'X';

    newRiver1->setMapCharToString(charToString1, string1);
    newRiver1->setTurtlePosition(glm::vec3(10, 140, 10));

<<<<<<< HEAD
    newRiver1->setTurtleIteration(2);
    newRiver1->setDistance(20);
    newRiver1->setWidth(30);
=======
    newRiver1->setTurtleIteration(1);
    newRiver1->setDistance(10);
    newRiver1->setWidth(10);
>>>>>>> 715fc8b33e239703627d70ec592b2ed32d57e4b9
    rivers.push(newRiver1);

    River* newRiver2 = new River(); //straight river
    QString string2 = "S<[<FX]>>FX";
    QChar charToString2 = 'X';
    newRiver2->setMapCharToString(charToString2, string2);
    newRiver2->setTurtlePosition(glm::vec3(20, 140, 20));
    newRiver2->setTurtleIteration(2);
    newRiver2->setWidth(10);
    newRiver2->setDistance(20);
    //rivers.push(newRiver2);

    for(River* r : rivers) {
        QString exp_path = createPath(r);
        drawSys(r, exp_path);
    }
}

QString Terrain::createPath(River *r) {
    QString startString = r->getAxiom();
    QString endString = "";
    int it = r->getTurtleIteration();
    for(int i = 0; i < it; i++) {
        endString = processString(r, startString);
        startString = endString;
    }
    return endString;
}

QString Terrain::processString(River* r, QString string) {
    QString newString = "";
    for(QChar c : string) {
        newString = newString + applyRules(r, c);
    }
    return newString;
}

QString Terrain::applyRules(River* r, QChar c) {
    QString newString = "";
    if(c == 'X') {
        QString mapped = r->getMapCharToString().find('X').value(); //get string attached
        newString = mapped;
    }
    else {
        newString = c;
    }
    return newString;
}

void Terrain::drawSys(River* r, QString string) {
    Turtle* t = r->getTurtle();
    QStack<glm::vec3> saved;
    int dist = r->getDistance();
    int width = r->getWidth();
    for(QChar c : string) {
        if(c == 'S') {
            saved.push(t->getPosition());
        } else if(c == '[') {
            //branch off
            float r1 = 1 * ((double) rand() / (RAND_MAX));
            float r2 = 1 * ((double) rand() / (RAND_MAX));
            dist -= r1;
            width -= r2;
        } else if( c == ']') {
            //bring it back to saved state
            float r1 =  5 * ((double) rand() / (RAND_MAX));
            float r2 =  3 * ((double) rand() / (RAND_MAX));
            dist += r1;
            width += r2;
            t->setPosition(saved.pop());
        } else if(c == 'F') {
            //now draw the line while moving some distance
            //Rule f = r->getMapCharToFptr().find('F').value();
            //f(dist);
            MoveAndDrawLine(r, t, dist, width);
        } else if (c == 'X') {
            //draw lake at the end of each
            MoveAndDrawLake(r, t);
        } else if (c == '>') {
            //change angle less
            glm::vec3 curr_or = t->getOrientation();
            float r1 =  ((double) rand() / (RAND_MAX));
            glm::vec3 rotated = glm::rotateY(curr_or, r1);
            t->setOrientation(glm::vec3(r1/2, 0, r1/2));
        } else if (c == '<') {
            glm::vec3 curr_or = t->getOrientation();
            float r1 = ((double) rand() / (RAND_MAX));
            glm::vec3 rotated = glm::rotateY(curr_or, r1);
            t->setOrientation(glm::vec3(r1/8, 0,-r1/8));
            //change angle less
        } else if (c == '+') {
            //change angle
            glm::vec3 curr_or = t->getOrientation();
            float r1 =( ((double) rand() / (RAND_MAX)));
            glm::vec3 rotated = glm::rotateY(curr_or, r1 + 2);
            t->setOrientation(glm::vec3(r1,0,r1));
        } else if (c == '-') {
            //change angle
            glm::vec3 curr_or = t->getOrientation();
            float r1 =(((double) rand() / (RAND_MAX)));
            glm::vec3 rotated = glm::rotateY(curr_or, r1 + 2);
            t->setOrientation(glm::vec3(-r1, 0, r1));
        }
    }
}

//move and draw lines per the expanded L-Sys
void Terrain::MoveAndDrawLine(River* r, Turtle* t, int dist, int width) {
    //create river blocks for distance worth
    for(int i = 0; i < dist; ++i) {
        glm::vec3 curr = t->getPosition();
        //create river blocks for width worth
        for(int i = -20; i < 20; ++i) {
            glm::vec3 curr_pos = glm::vec3(curr.x + i, curr.y, curr.z + (glm::abs(i) - 5));
            r->setBlocks(glm::vec3((int)curr_pos.x, (int)curr_pos.y, (int)curr_pos.z));
            for(int x = 0; i < 5; ++i) {
                r->setBlocks(glm::vec3((int)curr_pos.x, (int)curr_pos.y - x, (int)curr_pos.z));
            }
        }
        float r1 =  (1 * ((double) rand() / (RAND_MAX)));

        glm::vec3 ori = t->getOrientation();
        glm::vec3 new_position = curr + ori + glm::vec3(r1, 0, r1);
        t->setPosition(new_position);
    }
}

//move and draw lakes at the end of branches per the expanded L-Sys
void Terrain::MoveAndDrawLake(River* r, Turtle* t) {
    float r1 =  (5 * ((double) rand() / (RAND_MAX)));
    float r2 =  (3 * ((double) rand() / (RAND_MAX)));
    glm::vec3 curr = t->getPosition();

    for(int i = -r1; i < r1; ++i) {
        glm::vec3 curr_pos = glm::vec3(curr.x + i, curr.y, curr.z);
        for(int j = -r2; j < r2; ++j) {
            r->setBlocks(glm::vec3(curr_pos.x, curr_pos.y, curr_pos.z + j));
            for(int x = 0; x < 7; ++x) {
                r->setBlocks(glm::vec3(curr_pos.x, curr_pos.y - x, curr_pos.z + j));
            }
        }
    }
}


//caves
void Terrain::setCaves(OpenGLContext* context) {
    Cave* newCave1 = new Cave(); //delta river
    QString string1 = "S[-FX]+FX";
    QChar charToString1 = 'X';

    newCave1->setMapCharToString(charToString1, string1);
    newCave1->setTurtlePosition(glm::vec3(0, 128, 0));
    newCave1->setTurtleOrientation(glm::vec3(1, 0.4,1));

    newCave1->setTurtleIteration(2);
    newCave1->setDistance(30);
    newCave1->setWidth(10);
    caves.push(newCave1);

    for(Cave* c : caves) {
        QString exp_path = createPath(c);
        drawSys(c, exp_path);
    }
}

void Terrain::createEntrance() {
    for(Cave* c : caves) {
        std::vector<glm::vec3> caveBlocks = c->getBlocks();
        glm::vec3 startPos = caveBlocks.at(1);
        for(int i = 128; i < 256; ++i) {
            for(int j = -20; j < 20; ++i) {
                c->setBlocks(glm::vec3(startPos.x + j, i, startPos.z));
            }
        }
    }
}

//void Terrain::createChunksForCaves(OpenGLContext* context) {
//    QMutex mutex;

//    for(Cave* c : caves) {
//        std::vector<glm::vec3> caveBlocks = c->getBlocks();
//        for(glm::vec3 curr : caveBlocks) {
//            int x = ((int)(curr.x/64)) * 64;
//            int z = ((int)(curr.z/64)) * 64;
//            QPair<int,int> chunkOrig = QPair<int,int>();
//            chunkOrig.first = x;
//            chunkOrig.second = z;

//            glm::vec2 blockXZ = glm::vec2(curr.x, curr.z);

//            glm::vec2 topLeftCorner = glm::vec2(chunkOrig.first, chunkOrig.second);
//            glm::vec2 topRightCorner = glm::vec2(chunkOrig.first + 64, chunkOrig.second);
//            glm::vec2 bottomLeftCorner = glm::vec2(chunkOrig.first, chunkOrig.second + 64);
//            glm::vec2 bottomRightCorner = glm::vec2(chunkOrig.first + 64, chunkOrig.second + 64);

//            if(glm::distance(blockXZ, topLeftCorner) < 40 && !chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second - 64))) {
//                // generate to top left
//                std::vector<Chunk*> newChunks = createChunks(context, 4, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(glm::distance(blockXZ, topRightCorner) < 40 && !chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second - 64))) {
//                // generate to top right
//                std::vector<Chunk*> newChunks = createChunks(context, 5, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(glm::distance(blockXZ, bottomLeftCorner) < 40 && !chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second + 64))) {
//                // generate to bottom left
//                std::vector<Chunk*> newChunks = createChunks(context, 6, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(glm::distance(blockXZ, bottomRightCorner) < 40 && !chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second + 64))) {
//                // generate to bottom right
//                std::vector<Chunk*> newChunks = createChunks(context, 7, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            // Check direct horizontal and vertical positions
//            if(curr.x > (chunkOrig.first + 64) - 40 && !chunks.contains(QPair<int, int>(chunkOrig.first + 64, chunkOrig.second))) {
//                // generate on right side
//                std::vector<Chunk*> newChunks = createChunks(context, 1, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(curr.x < (chunkOrig.first) + 40 && !chunks.contains(QPair<int, int>(chunkOrig.first - 64, chunkOrig.second))) {
//                // generate on left side
//                std::vector<Chunk*> newChunks = createChunks(context, 0, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(curr.z > (chunkOrig.second + 64) - 40 && !chunks.contains(QPair<int, int>(chunkOrig.first, chunkOrig.second + 64))) {
//                // generate on top side
//                std::vector<Chunk*> newChunks = createChunks(context, 3, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//            if(curr.z < (chunkOrig.second) + 40 && !chunks.contains(QPair<int, int>(chunkOrig.first, chunkOrig.second - 64))) {
//                // generate on bottom side
//                std::vector<Chunk*> newChunks = createChunks(context, 2, curr);
//                // Make a QRunnable for each chunk
//                for(Chunk* newChunk : newChunks) {
//                    chunks.insert(QPair<int, int>(x, z), newChunk);
//                    Chunker* newChunker = new Chunker(&mutex, this, newChunk);
//                    QThreadPool::globalInstance()->start(newChunker);
//                }
//                QThreadPool::globalInstance()->waitForDone();
//                for(Chunk* newChunk : newChunks) {
//                    newChunk->createMembers();
//                }
//            }
//        }
//    }
//}

void Terrain::createBlocksForCaves(OpenGLContext* context) {
    for(Cave* c : caves) {
        std::vector<glm::vec3> caveBlocks = c->getBlocks();
        for(glm::vec3 curr : caveBlocks) {
            int x = curr.x;
            int y = curr.y;
            int z = curr.z;
            if(x > worldBounds[0] && z > worldBounds[2] && x < worldBounds[1] && z < worldBounds[3] && y > 0 && y < 256) {
                if(getBlockAt(x, y, z) != LAVA) {
                    setBlockAt(x, y, z, EMPTY);
                    for(int i = -5; i < 5; ++i) {
                        for(int j = -5; j < 5; ++j) {
                            glm::vec3 pos = glm::vec3(x + i, y + j, z);
                            if(pos.x > worldBounds[0] && pos.z > worldBounds[2] && pos.x < worldBounds[1] && pos.z < worldBounds[3] && pos.y > 0) {
                                float r1 =  (2 * ((double) rand() / (RAND_MAX)));
                                if(r1 < 0.5) {
                                    setBlockAt(pos.x, pos.y, pos.z, COAL);
                                }
                            }
                            else {
                                setBlockAt(pos.x, pos.y, pos.z, EMPTY);
                            }
                        }
                    }

                    if(y < 117) {
                        setBlockAt(x, y , z, LAVA);
                    }
                }
            }
        }
    }
}

QString Terrain::createPath(Cave *r) {
    QString startString = r->getAxiom();
    QString endString = "";
    int it = r->getTurtleIteration();
    for(int i = 0; i < it; i++) {
        endString = processString(r, startString);
        startString = endString;
    }
    return endString;
}

QString Terrain::processString(Cave* r, QString string) {
    QString newString = "";
    for(QChar c : string) {
        newString = newString + applyRules(r, c);
    }
    return newString;
}

QString Terrain::applyRules(Cave* r, QChar c) {
    QString newString = "";
    if(c == 'X') {
        QString mapped = r->getMapCharToString().find('X').value(); //get string attached
        newString = mapped;
    }
    else {
        newString = c;
    }
    return newString;
}

void Terrain::drawSys(Cave* r, QString string) {
    Turtle* t = r->getTurtle();
    QStack<glm::vec3> saved;
    int dist = r->getDistance();
    int width = r->getWidth();
    for(QChar c : string) {
        if(c == 'S') {
            saved.push(t->getPosition());
        } else if(c == '[') {
            //branch off
            float r1 = 1 * ((double) rand() / (RAND_MAX));
            float r2 = 1 * ((double) rand() / (RAND_MAX));
            dist -= r1;
            width -= r2;
        } else if( c == ']') {
            //bring it back to saved state
            float r1 =  5 * ((double) rand() / (RAND_MAX));
            float r2 =  3 * ((double) rand() / (RAND_MAX));
            dist += r1;
            width += r2;
            t->setPosition(saved.pop());
        } else if(c == 'F') {
            //now draw the line while moving some distance
            //Rule f = r->getMapCharToFptr().find('F').value();
            //f(dist);
            MoveAndDrawLine(r, t, dist, width);
        } else if (c == 'X') {
            //draw cave at the end of each
            MoveAndDrawCavern(r, t);
        } else if (c == '>') {
            //change angle less
            glm::vec3 curr_or = t->getOrientation();
            float r1 =  ((double) rand() / (RAND_MAX));
            glm::vec3 rotated = glm::rotateY(curr_or, r1);
            t->setOrientation(glm::vec3(r1/2, 0, r1/2));
        } else if (c == '<') {
            glm::vec3 curr_or = t->getOrientation();
            float r1 = ((double) rand() / (RAND_MAX));
            glm::vec3 rotated = glm::rotateY(curr_or, r1);
            t->setOrientation(glm::vec3(r1/8, 0,-r1/8));
            //change angle less
        } else if (c == '+') {
            //change angle
            glm::vec3 curr_or = t->getOrientation();
            float r1 =( ((double) rand() / (RAND_MAX)));
            glm::vec3 rotated = glm::rotateY(curr_or, r1 + 2);
            t->setOrientation(glm::vec3(r1,0,r1));
        } else if (c == '-') {
            //change angle
            glm::vec3 curr_or = t->getOrientation();
            float r1 =(((double) rand() / (RAND_MAX)));
            glm::vec3 rotated = glm::rotateY(curr_or, r1 + 2);
            t->setOrientation(glm::vec3(-r1, 0, r1));
        }
    }
}

//move and draw lines per the expanded L-Sys
void Terrain::MoveAndDrawLine(Cave* r, Turtle* t, int dist, int width) {
    //create river blocks for distance worth
    for(int i = 0; i < dist; ++i) {
        glm::vec3 curr = t->getPosition();
        //create river blocks for width worth
        for(int i = -20; i < 20; ++i) {
            glm::vec3 curr_pos = glm::vec3(curr.x + i, curr.y, curr.z + (glm::abs(i) - 5));
            r->setBlocks(glm::vec3((int)curr_pos.x, (int)curr_pos.y, (int)curr_pos.z));
            for(int x = 0; i < 5; ++i) {
                r->setBlocks(glm::vec3((int)curr_pos.x, (int)curr_pos.y - x, (int)curr_pos.z));
                //std::cout<<"current block"<<curr_pos.x;
            }
        }
        float r1 =  (2 * ((double) rand() / (RAND_MAX)));

        glm::vec3 ori = t->getOrientation();
        glm::vec3 new_position = curr + ori + glm::vec3(r1, 0, r1);
        t->setPosition(new_position);
    }
}

//move and draw lakes at the end of branches per the expanded L-Sys
void Terrain::MoveAndDrawCavern(Cave* r, Turtle* t) {
    float r1 =  (10 * ((double) rand() / (RAND_MAX)));
    float r2 =  (5 * ((double) rand() / (RAND_MAX)));
    glm::vec3 curr = t->getPosition();

    for(int i = -r1; i < r1; ++i) {
        glm::vec3 curr_pos = glm::vec3(curr.x + i, curr.y, curr.z);
        for(int j = -r2; j < r2; ++j) {
            r->setBlocks(glm::vec3(curr_pos.x, curr_pos.y, curr_pos.z + j));
            for(int x = 0; x < 7; ++x) {
                r->setBlocks(glm::vec3(curr_pos.x, curr_pos.y - x, curr_pos.z + j));
            }
        }
    }
}



Chunk::Chunk(OpenGLContext* context, QPair<int, int> origin, Terrain *t)
    : Drawable(context), origin(origin), terrain(t), m_blocks(), drawOpaque(true)
{}

Chunk::~Chunk() {
    // Do nothing (for now)
}

glm::vec4 Chunk::getColor(BlockType block) {
    switch(block)
    {
    case DIRT:
        return glm::vec4(121.f, 85.f, 58.f, 255.f) / 255.f;
        break;
    case GRASS:
        return glm::vec4(95.f, 159.f, 53.f, 255.f) / 255.f;
        break;
    case STONE:
        return glm::vec4(0.5f, .5f, .5f, 1.0f);
        break;
    case LAVA:
        return glm::vec4(1.f, .545f, .16f, 1.f);
        break;
    case LEAF:
        return glm::vec4(.321f, 1.f, .16f, 1.f);
        break;
    case BEDROCK:
        return glm::vec4(.219f, .223, .219, 1.f);
        break;
    case WATER:
        return glm::vec4(.349f, .258f, .941f, .7f);
        break;
    case ICE:
        return glm::vec4(.733f, .921f, .968f, 1.f);
        break;
    }

}

void Chunk::create() {

    std::vector<GLuint> indices;
    // positions, normals, UVs
    std::vector<float> data;
    std::vector<float> dataTrans;

    int startIdx = 0;

    int origX = origin.first;
    int origZ = origin.second;

    // For EVERY block in the chunk
    for(int x = 0; x < 16; ++x) {
        for(int z = 0; z < 16; ++z) {
            for(int y = 0; y < 256; ++y) {

                // Temp vectors to store pos, norms, uvs individually, will then be pushed to DATA vector
                std::vector<glm::vec4> tempPositions;
                std::vector<glm::vec4> tempNormals;
                std::vector<glm::vec2> tempUvs;
                std::vector<float> tempBlocks;

                std::vector<glm::vec4> tempPositionsTrans;
                std::vector<glm::vec4> tempNormalsTrans;
                std::vector<glm::vec2> tempUvsTrans;
                std::vector<float> tempBlocksTrans;

                //(x, y, z) represent "Chunk space" position of each block
                // These will represent the world space position of a block
                int xWorld = origX + x;
                int zWorld = origZ + z;
                // Check each neighbor of chunk

                BlockType block = this->getBlockAt(x, y, z);
                BlockType neighbor;
                // For drawing water and lava with a small dip -- not yet implemented

                // FRONT FACE:
                if((z + 1) > 15) {
                    // make neighbor in front chunk
                    // Same y coord, max z of chunk, same x

                    // See if zWorld + 1 is outside the dimensions of the terrain
                    if(!terrain->chunkAt(xWorld, zWorld + 1)) {
                        // If so, just consider the neighboring block to be empty so
                        // we can draw the face.
                        neighbor = DIRT;
                    }
                    else {
                        neighbor = terrain->getBlockAt(xWorld, y, zWorld + 1);
                    }
                }
                else {
                    neighbor = this->getBlockAt(x, y, z + 1);
                }
                // Assuming we have the appropriate neighbor...
                // Make the face!
                // We'd like to calculate positions, normals, and indices for each FACE
                // But only if the neighbor is empty!

                if(((neighbor == EMPTY) || (neighbor == WATER && block != WATER) || (neighbor == LAVA && block != LAVA)) && block != EMPTY) {
                    if(block == WATER || block == ICE) {
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y + 1.f, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y + 1.f, z + 1.f, 1.f));
                    }
                    else {
                        tempPositions.push_back(glm::vec4(x + 1.f, y + 1.f, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x, y, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x, y + 1.f, z + 1.f, 1.f));
                    }


                    // 4 normals for one face
                    for(int i = 0; i < 4; ++i) {
                        if(block == WATER || block == ICE) {
                            tempNormalsTrans.push_back(glm::vec4(0.f, 0.f, 1.f, 0.f));
                        }
                        else {
                            tempNormals.push_back(glm::vec4(0.f, 0.f, 1.f, 0.f));
                        }

                        switch(block)
                        {
                        case GRASS:
                            tempBlocks.push_back(1.f);
                            break;
                        case DIRT:
                            tempBlocks.push_back(1.f);
                            break;
                        case STONE:
                            tempBlocks.push_back(5.f);
                            break;
                        case WATER:
                            tempBlocksTrans.push_back(10.f);
                            break;
                        case LAVA:
                            tempBlocks.push_back(10.f);
                            break;
                        case WOOD:
                            tempBlocks.push_back(1.f);
                            break;
                        case LEAF:
                            tempBlocks.push_back(1.f);
                            break;
                        case BEDROCK:
                            tempBlocks.push_back(5.f);
                            break;
                        case ICE:
                            tempBlocksTrans.push_back(5.f);
                            break;
                        case COAL:
                            tempBlocks.push_back(5.f);
                            break;
                        }
                    }

                    // And also 4 for color
                    switch(block)
                    {
                    case DIRT:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        break;
                    case GRASS:
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        break;
                    case STONE:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 1.f));
                        break;
                    case WATER:
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 4.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
                        break;
                    case LAVA:
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 2.f / 16.f));
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
                        break;
                    case WOOD:
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        break;
                    case LEAF:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 13.f / 16.f));
                        break;
                    case BEDROCK:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        break;
                    case ICE:
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 12.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 12.f / 16.f));
                        break;
                    case COAL:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        break;
                    }

                    // This face consists of two triangles -- generate indices appropriately
                    for(int j = 0; j < 2; ++j) { // Per triangle
                        indices.push_back(startIdx);
                        indices.push_back(startIdx + j + 1);
                        indices.push_back(startIdx + j + 2);
                    }
                    startIdx = startIdx + 4; // Should be the very last index
                }
                // Otherwise, we don't bother generating a face, as it won't be seen

                // LEFT FACE:
                if((x - 1) < 0) {
                    if(!terrain->chunkAt(xWorld - 1, zWorld)) {
                        neighbor = STONE;
                    }
                    else {
                        neighbor = terrain->getBlockAt(xWorld - 1, y, zWorld);
                    }
                }
                else {
                    neighbor = this->getBlockAt(x - 1, y, z);
                }

                if(((neighbor == EMPTY) || (neighbor == WATER && block != WATER) || (neighbor == LAVA && block != LAVA)) && block != EMPTY) {
                    if(block == WATER || block == ICE) {
                        tempPositionsTrans.push_back(glm::vec4(x, y + 1.f, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y + 1.f, z, 1.f));
                    }
                    else {
                        tempPositions.push_back(glm::vec4(x, y + 1.f, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x, y, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x, y, z, 1.f));
                        tempPositions.push_back(glm::vec4(x, y + 1.f, z, 1.f));
                    }



                    // 4 normals for one face
                    for(int i = 0; i < 4; ++i) {
                        if(block == WATER || block == ICE) {
                            tempNormalsTrans.push_back(glm::vec4(-1.f, 0.f, 0.f, 0.f));
                        }
                        else {
                            tempNormals.push_back(glm::vec4(-1.f, 0.f, 0.f, 0.f));
                        }

                        switch(block)
                        {
                        case GRASS:
                            tempBlocks.push_back(1.f);
                            break;
                        case DIRT:
                            tempBlocks.push_back(1.f);
                            break;
                        case STONE:
                            tempBlocks.push_back(5.f);
                            break;
                        case WATER:
                            tempBlocksTrans.push_back(10.f);
                            break;
                        case LAVA:
                            tempBlocks.push_back(10.f);
                            break;
                        case WOOD:
                            tempBlocks.push_back(1.f);
                            break;
                        case LEAF:
                            tempBlocks.push_back(1.f);
                            break;
                        case BEDROCK:
                            tempBlocks.push_back(5.f);
                            break;
                        case ICE:
                            tempBlocksTrans.push_back(5.f);
                            break;
                        case COAL:
                            tempBlocks.push_back(5.f);
                            break;
                        }
                    }

                    // And 4 for color
                    switch(block)
                    {
                    case DIRT:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        break;
                    case GRASS:
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        break;
                    case STONE:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 1.f));
                        break;
                    case WATER:
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 4.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
                        break;
                    case LAVA:
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 2.f / 16.f));
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
                        break;
                    case WOOD:
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        break;
                    case LEAF:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 13.f / 16.f));
                        break;
                    case BEDROCK:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        break;
                    case ICE:
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 12.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 12.f / 16.f));
                        break;
                    case COAL:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        break;
                    }

                    // This face consists of two triangles -- generate indices appropriately
                    for(int j = 0; j < 2; ++j) { // Per triangle
                        indices.push_back(startIdx);
                        indices.push_back(startIdx + j + 1);
                        indices.push_back(startIdx + j + 2);
                    }
                    startIdx = startIdx + 4; // Should be the very last index
                }


                // BACK FACE:
                if((z - 1) < 0) {
                    if(!terrain->chunkAt(xWorld, zWorld - 1)) {
                        neighbor = STONE;
                    }
                    else {
                        neighbor = terrain->getBlockAt(xWorld, y, zWorld - 1);
                    }
                }
                else {
                    neighbor = this->getBlockAt(x, y, z - 1);
                }

                if(((neighbor == EMPTY) || (neighbor == WATER && block != WATER) || (neighbor == LAVA && block != LAVA)) && block != EMPTY) {
                    if(block == WATER || block == ICE) {
                        tempPositionsTrans.push_back(glm::vec4(x, y + 1.f, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y + 1.f, z, 1.f));
                    }
                    else {
                        tempPositions.push_back(glm::vec4(x, y + 1.f, z, 1.f));
                        tempPositions.push_back(glm::vec4(x, y, z, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y, z, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y + 1.f, z, 1.f));
                    }



                    // 4 normals for one face
                    for(int i = 0; i < 4; ++i) {
                        if(block == WATER || block == ICE) {
                            tempNormalsTrans.push_back(glm::vec4(0.f, 0.f, -1.f, 0.f));
                        }
                        else {
                            tempNormals.push_back(glm::vec4(0.f, 0.f, -1.f, 0.f));
                        }

                        switch(block)
                        {
                        case GRASS:
                            tempBlocks.push_back(1.f);
                            break;
                        case DIRT:
                            tempBlocks.push_back(1.f);
                            break;
                        case STONE:
                            tempBlocks.push_back(5.f);
                            break;
                        case WATER:
                            tempBlocksTrans.push_back(10.f);
                            break;
                        case LAVA:
                            tempBlocks.push_back(10.f);
                            break;
                        case WOOD:
                            tempBlocks.push_back(1.f);
                            break;
                        case LEAF:
                            tempBlocks.push_back(1.f);
                            break;
                        case BEDROCK:
                            tempBlocks.push_back(5.f);
                            break;
                        case ICE:
                            tempBlocksTrans.push_back(5.f);
                            break;
                        case COAL:
                            tempBlocks.push_back(5.f);
                            break;
                        }
                    }

                    // And 4 for color
                    switch(block)
                    {
                    case DIRT:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        break;
                    case GRASS:
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        break;
                    case STONE:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 1.f));
                        break;
                    case WATER:
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 4.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
                        break;
                    case LAVA:
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 2.f / 16.f));
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
                        break;
                    case WOOD:
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        break;
                    case LEAF:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 13.f / 16.f));
                        break;
                    case BEDROCK:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        break;
                    case ICE:
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 12.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 12.f / 16.f));
                        break;
                    case COAL:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        break;
                    }

                    // This face consists of two triangles -- generate indices appropriately
                    for(int j = 0; j < 2; ++j) { // Per triangle
                        indices.push_back(startIdx);
                        indices.push_back(startIdx + j + 1);
                        indices.push_back(startIdx + j + 2);
                    }
                    startIdx = startIdx + 4; // Should be the very last index
                }

                // RIGHT FACE:
                if((x + 1) > 15) {
                    if(!terrain->chunkAt(xWorld + 1, zWorld)) {
                        neighbor = STONE;
                    }
                    else {
                        neighbor = terrain->getBlockAt(xWorld + 1, y, zWorld);
                    }
                }
                else {
                    neighbor = this->getBlockAt(x + 1, y, z);
                }

                if(((neighbor == EMPTY) || (neighbor == WATER && block != WATER) || (neighbor == LAVA && block != LAVA)) && block != EMPTY) {
                    if(block == WATER || block == ICE) {
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y + 1.f, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y + 1.f, z + 1.f, 1.f));
                    }
                    else {
                        tempPositions.push_back(glm::vec4(x + 1.f, y + 1.f, z, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y, z, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y + 1.f, z + 1.f, 1.f));
                    }



                    // 4 normals for one face
                    for(int i = 0; i < 4; ++i) {
                        if(block == WATER || block == ICE) {
                            tempNormalsTrans.push_back(glm::vec4(1.f, 0.f, 0.f, 0.f));
                        }
                        else {
                            tempNormals.push_back(glm::vec4(1.f, 0.f, 0.f, 0.f));
                        }

                        switch(block)
                        {
                        case GRASS:
                            tempBlocks.push_back(1.f);
                            break;
                        case DIRT:
                            tempBlocks.push_back(1.f);
                            break;
                        case STONE:
                            tempBlocks.push_back(5.f);
                            break;
                        case WATER:
                            tempBlocksTrans.push_back(10.f);
                            break;
                        case LAVA:
                            tempBlocks.push_back(10.f);
                            break;
                        case WOOD:
                            tempBlocks.push_back(1.f);
                            break;
                        case LEAF:
                            tempBlocks.push_back(1.f);
                            break;
                        case BEDROCK:
                            tempBlocks.push_back(5.f);
                            break;
                        case ICE:
                            tempBlocksTrans.push_back(5.f);
                            break;
                        case COAL:
                            tempBlocks.push_back(5.f);
                            break;
                        }
                    }

                    // And 4 for color
                    switch(block)
                    {
                    case DIRT:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        break;
                    case GRASS:
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        break;
                    case STONE:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 1.f));
                        break;
                    case WATER:
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 4.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
                        break;
                    case LAVA:
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 2.f / 16.f));
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
                        break;
                    case WOOD:
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(4.f / 16.f, 15.f / 16.f));
                        break;
                    case LEAF:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 13.f / 16.f));
                        break;
                    case BEDROCK:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        break;
                    case ICE:
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 12.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 12.f / 16.f));
                        break;
                    case COAL:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        break;
                    }

                    // This face consists of two triangles -- generate indices appropriately
                    for(int j = 0; j < 2; ++j) { // Per triangle
                        indices.push_back(startIdx);
                        indices.push_back(startIdx + j + 1);
                        indices.push_back(startIdx + j + 2);
                    }
                    startIdx = startIdx + 4; // Should be the very last index
                }

                // There are no chunks above and below!
                // TOP FACE:
                if((y + 1) > 255) {
                    // There is no block above, so let's just draw the face anyway
                    neighbor = EMPTY;
                }
                else {
                    neighbor = this->getBlockAt(x, y + 1, z);
                }

                if(((neighbor == EMPTY) || (neighbor == WATER && block != WATER) || (neighbor == LAVA && block != LAVA)) && block != EMPTY) {
                    if(block == WATER || block == ICE) {
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y + 1.f, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y + 1.f, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y + 1.f, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y + 1.f, z, 1.f));
                    }
                    else {
                        tempPositions.push_back(glm::vec4(x + 1.f, y + 1.f, z, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y + 1.f, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x, y + 1.f, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x, y + 1.f, z, 1.f));
                    }


                    // 4 normals for one face
                    for(int i = 0; i < 4; ++i) {
                        if(block == WATER || block == ICE) {
                            tempNormalsTrans.push_back(glm::vec4(0.f, 1.f, 0.f, 0.f));
                        }
                        else {
                            tempNormals.push_back(glm::vec4(0.f, 1.f, 0.f, 0.f));
                        }

                        switch(block)
                        {
                        case GRASS:
                            tempBlocks.push_back(1.f);
                            break;
                        case DIRT:
                            tempBlocks.push_back(1.f);
                            break;
                        case STONE:
                            tempBlocks.push_back(5.f);
                            break;
                        case WATER:
                            tempBlocksTrans.push_back(10.f);
                            break;
                        case LAVA:
                            tempBlocks.push_back(10.f);
                            break;
                        case WOOD:
                            tempBlocks.push_back(1.f);
                            break;
                        case LEAF:
                            tempBlocks.push_back(1.f);
                            break;
                        case BEDROCK:
                            tempBlocks.push_back(5.f);
                            break;
                        case ICE:
                            tempBlocksTrans.push_back(5.f);
                            break;
                        case COAL:
                            tempBlocks.push_back(5.f);
                            break;
                        }
                    }

                    // And 4 for color
                    switch(block)
                    {
                    case DIRT:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        break;
                    case GRASS:
                        tempUvs.push_back(glm::vec2(9.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(9.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(8.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(8.f / 16.f, 14.f / 16.f));
                        break;
                    case STONE:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 1.f));
                        break;
                    case WATER:
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 4.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
                        break;
                    case LAVA:
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 2.f / 16.f));
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
                        break;
                    case WOOD:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 15.f / 16.f));
                        break;
                    case LEAF:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 13.f / 16.f));
                        break;
                    case BEDROCK:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        break;
                    case ICE:
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 12.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 12.f / 16.f));
                        break;
                    case COAL:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        break;
                    }

                    // This face consists of two triangles -- generate indices appropriately
                    for(int j = 0; j < 2; ++j) { // Per triangle
                        indices.push_back(startIdx);
                        indices.push_back(startIdx + j + 1);
                        indices.push_back(startIdx + j + 2);
                    }
                    startIdx = startIdx + 4; // Should be the very last index
                }

                // BOTTOM FACE:
                if((y - 1) < 0) {
                    // There is no block below, so let's just draw the face anyway
                    neighbor = EMPTY;
                }
                else {
                    neighbor = this->getBlockAt(z, y - 1, z);
                }

                if(((neighbor == EMPTY) || (neighbor == WATER && block != WATER) || (neighbor == LAVA && block != LAVA)) && block != EMPTY) {
                    if(block == WATER || block == ICE) {
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y, z + 1.f, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x + 1.f, y, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y, z, 1.f));
                        tempPositionsTrans.push_back(glm::vec4(x, y, z + 1.f, 1.f));
                    }
                    else {
                        tempPositions.push_back(glm::vec4(x + 1.f, y, z + 1.f, 1.f));
                        tempPositions.push_back(glm::vec4(x + 1.f, y, z, 1.f));
                        tempPositions.push_back(glm::vec4(x, y, z, 1.f));
                        tempPositions.push_back(glm::vec4(x, y, z + 1.f, 1.f));
                    }


                    // 4 normals for one face
                    for(int i = 0; i < 4; ++i) {
                        if(block == WATER || block == ICE) {
                            tempNormalsTrans.push_back(glm::vec4(0.f, -1.f, 0.f, 0.f));
                        }
                        else {
                            tempNormals.push_back(glm::vec4(0.f, -1.f, 0.f, 0.f));
                        }

                        switch(block)
                        {
                        case GRASS:
                            tempBlocks.push_back(1.f);
                            break;
                        case DIRT:
                            tempBlocks.push_back(1.f);
                            break;
                        case STONE:
                            tempBlocks.push_back(5.f);
                            break;
                        case WATER:
                            tempBlocksTrans.push_back(10.f);
                            break;
                        case LAVA:
                            tempBlocks.push_back(10.f);
                            break;
                        case WOOD:
                            tempBlocks.push_back(1.f);
                            break;
                        case LEAF:
                            tempBlocks.push_back(1.f);
                            break;
                        case BEDROCK:
                            tempBlocks.push_back(5.f);
                            break;
                        case ICE:
                            tempBlocksTrans.push_back(5.f);
                            break;
                        case COAL:
                            tempBlocks.push_back(5.f);
                            break;
                        }
                    }

                    // And 4 for color
                    switch(block)
                    {
                    case DIRT:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        break;
                    case GRASS:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        break;
                    case STONE:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 1.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 1.f));
                        break;
                    case WATER:
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 4.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(14.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 3.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(13.f / 16.f, 4.f / 16.f));
                        break;
                    case LAVA:
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 2.f / 16.f));
                        tempUvs.push_back(glm::vec2(14.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 1.f / 16.f));
                        tempUvs.push_back(glm::vec2(13.f / 16.f, 2.f / 16.f));
                        break;
                    case WOOD:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 15.f / 16.f));
                        break;
                    case LEAF:
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(6.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 12.f / 16.f));
                        tempUvs.push_back(glm::vec2(5.f / 16.f, 13.f / 16.f));
                        break;
                    case BEDROCK:
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 15.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(1.f / 16.f, 15.f / 16.f));
                        break;
                    case ICE:
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 12.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(4.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 11.f / 16.f));
                        tempUvsTrans.push_back(glm::vec2(3.f / 16.f, 12.f / 16.f));
                        break;
                    case COAL:
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 14.f / 16.f));
                        tempUvs.push_back(glm::vec2(3.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 13.f / 16.f));
                        tempUvs.push_back(glm::vec2(2.f / 16.f, 14.f / 16.f));
                        break;
                    }

                    // This face consists of two triangles -- generate indices appropriately
                    for(int j = 0; j < 2; ++j) { // Per triangle
                        indices.push_back(startIdx);
                        indices.push_back(startIdx + j + 1);
                        indices.push_back(startIdx + j + 2);
                    }
                    startIdx = startIdx + 4; // Should be the very last index
                }
                // Now, push temp vertex data to big ol data vector.  These are vertices for each BLOCK.
                for(int g = 0; g < tempPositions.size(); g++) {
                    glm::vec4 position = tempPositions[g];
                    glm::vec4 normal = tempNormals[g];
                    glm::vec2 uv = tempUvs[g];
                    float blockVal = tempBlocks[g];
                    data.push_back(position.x);
                    data.push_back(position.y);
                    data.push_back(position.z);
                    data.push_back(position.w);
                    data.push_back(normal.x);
                    data.push_back(normal.y);
                    data.push_back(normal.z);
                    data.push_back(normal.w);
                    data.push_back(uv.x);
                    data.push_back(uv.y);
                    data.push_back(blockVal);
                }
                for(int g = 0; g < tempPositionsTrans.size(); g++) {
                    glm::vec4 position = tempPositionsTrans[g];
                    glm::vec4 normal = tempNormalsTrans[g];
                    glm::vec2 uv = tempUvsTrans[g];
                    float blockVal = tempBlocksTrans[g];
                    dataTrans.push_back(position.x);
                    dataTrans.push_back(position.y);
                    dataTrans.push_back(position.z);
                    dataTrans.push_back(position.w);
                    dataTrans.push_back(normal.x);
                    dataTrans.push_back(normal.y);
                    dataTrans.push_back(normal.z);
                    dataTrans.push_back(normal.w);
                    dataTrans.push_back(uv.x);
                    dataTrans.push_back(uv.y);
                    dataTrans.push_back(blockVal);
                }
            }
        }
    }

    // NOW BIND
    count = indices.size();

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.

    generateData();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufData);
    context->glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), data.data(), GL_STATIC_DRAW);

    generateDataTrans();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufDataTrans);
    context->glBufferData(GL_ARRAY_BUFFER, dataTrans.size() * sizeof(GL_FLOAT), dataTrans.data(), GL_STATIC_DRAW);


}

void Chunk::createMembers() {
    count = this->indices.size();

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.

    generateData();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufData);
    context->glBufferData(GL_ARRAY_BUFFER, this->data.size() * sizeof(GL_FLOAT), this->data.data(), GL_STATIC_DRAW);

    generateDataTrans();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufDataTrans);
    context->glBufferData(GL_ARRAY_BUFFER, this->dataTrans.size() * sizeof(GL_FLOAT), this->dataTrans.data(), GL_STATIC_DRAW);
}

BlockType Chunk::getBlockAt(int x, int y, int z) const {
    return m_blocks[x + y * 16 + z * 16 * 256];
}

BlockType& Chunk::setBlockAt(int x, int y, int z) {
    return m_blocks[x + y * 16 + z * 16 * 256];
}

std::vector<Chunk*> Terrain::createChunks(OpenGLContext *context, int side, glm::vec3 pos) {
    // Create new chunks
    // The origins of these chunks depends on which border we're expanding from, so this should be
    // passed in.

    std::vector<Chunk*> testChunks;
    int startX;
    int startZ;

    if(side == 0) {
        // expand left
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64) - 64;
        startZ = ((posZ / 64) * 64);

        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }

        this->dimensions = glm::ivec3(dimensions[0] + 64, dimensions[1], dimensions[2]);
        this->worldBounds = glm::ivec4(worldBounds[0] - 64, worldBounds[1], worldBounds[2], worldBounds[3]);
    }
    if(side == 1) {
        // expand rightways
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64) + 64;
        startZ = ((posZ / 64) * 64);


        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }

        // expand this terrains dimensions and borders
        this->dimensions = glm::ivec3(dimensions[0] + 64, dimensions[1], dimensions[2]);
        this->worldBounds = glm::ivec4(worldBounds[0], worldBounds[1] + 64, worldBounds[2], worldBounds[3]);

    }
    if(side == 2) {
        // expand up
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64);
        startZ = ((posZ / 64) * 64) - 64;


        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }

        this->dimensions = glm::ivec3(dimensions[0], dimensions[1], dimensions[2] + 64);
        this->worldBounds = glm::ivec4(worldBounds[0], worldBounds[1], worldBounds[2] - 64, worldBounds[3]);
    }
    if(side == 3) {
        // expand down
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64);
        startZ = ((posZ / 64) * 64) + 64;


        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }

        this->dimensions = glm::ivec3(dimensions[0], dimensions[1], dimensions[2] + 64);
        this->worldBounds = glm::ivec4(worldBounds[0], worldBounds[1], worldBounds[2], worldBounds[3] + 64);
    }
    if(side == 4) {
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64) - 64;
        startZ = ((posZ / 64) * 64) - 64;


        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }
    }
    if(side == 5) {
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64) + 64;
        startZ = ((posZ / 64) * 64) - 64;


        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }
    }
    if(side == 6) {
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64) - 64;
        startZ = ((posZ / 64) * 64) + 64;


        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }
    }
    if(side == 7) {
        int posX = pos[0];
        int posZ = pos[2];
        startX = ((posX / 64) * 64) + 64;
        startZ = ((posZ / 64) * 64) + 64;


        for(int x = startX; x < startX + 64; x += 16) {
            for(int z = startZ; z < startZ + 64; z += 16) {
                Chunk* newChunk = new Chunk(context, QPair<int, int>(x, z), this);
                testChunks.push_back(newChunk);
                chunks.insert(QPair<int, int>(x, z), newChunk);
            }
        }
    }

    //Initialize each block to empty

    for(int x = startX; x < startX + 64; ++x) {
        for(int z = startZ; z < startZ + 64; ++z) {
            for(int y = 0; y < 256; y++) {
                setBlockAt(x, y, z, EMPTY);
            }
        }
    }


    return testChunks;
}

bool Terrain::chunkAt(int x, int z) {
    int origX;
    int origZ;
    if(x < 0) {
        origX = ((x / 16) * 16);
        if(x % 16 != 0) {
            origX -= 16;
        }
    }
    else {
        origX = (x / 16) * 16;
    }
    if(z < 0) {
        origZ = ((z / 16) * 16);
        if(z % 16 != 0) {
            origZ -= 16;
        }
    }
    else {
        origZ = (z / 16) * 16;
    }
    QPair<int, int> origin = QPair<int, int>(origX, origZ);
    return chunks.contains(origin);
}

void Terrain::
updateChunks(int x, int z) {
    int origX;
    int origZ;
    if(x < 0) {
        origX = ((x / 64) * 64);
        if(x % 64 != 0) {
            origX -= 64;
        }
    }
    else {
        origX = (x / 64) * 64;
    }
    if(z < 0) {
        origZ = ((z / 64) * 64);
        if(z % 64 != 0) {
            origZ -= 64;
        }
    }
    else {
        origZ = (z / 64) * 64;
    }
    QPair<int, int> origin = QPair<int, int>(origX, origZ);
    for(int x = origX; x < origX + (4 * 16); x += 16) {
        for(int z = origZ; z < origZ + (4 * 16); z += 16) {
            Chunk* chunk = this->chunks[origin];
            chunk->create();
        }
    }
}
