#pragma once
#include <QList>
#include <QHash>
#include <QPair>
#include <la.h>
#include <math.h>
#include <drawable.h>
#include <iostream>
#include <array>
#include <scene/river.h>
#include <scene/cave.h>
#include <QStack>

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.
enum BlockType : unsigned char
{
    EMPTY, GRASS, DIRT, STONE, LAVA, WOOD, LEAF, BEDROCK, WATER, ICE, COAL
};

// Forward declaration so we can use Terrain::getBlockAt in Chunk class
class Terrain;

class Chunk : public Drawable {
public:
    // VBO data for one chunk
    std::vector<GLuint> indices;
    std::vector<float> data;
    std::vector<float> dataTrans;

    Chunk(OpenGLContext* context, QPair<int, int> origin, Terrain* t);
    virtual ~Chunk();
    void create() override;
    void createMembers();

    glm::vec4 getColor(BlockType block);

    std::array<BlockType, 65536> m_blocks;

    BlockType getBlockAt(int x, int y, int z) const;
    BlockType& setBlockAt(int x, int y, int z);

    // World space origin of chunk
    QPair<int, int> origin;
    // The terrain this chunk belongs to
    Terrain* terrain;

    bool drawOpaque;
};

class River;
class Cave;

class Terrain
{
public:

    QStack<River*> rivers;
    QStack<Cave*> caves;
    // Maps (x,z) world space to a Chunk (the coord being its origin)
    QHash<QPair<int, int>, Chunk*> chunks;

    Terrain();

    void CreateTestScene(OpenGLContext* context);

    glm::ivec3 dimensions;

    // The world space bounds of this terrain, in order (xmin, xmax, zmin, zmax)
    glm::ivec4 worldBounds;

    float fractal(glm::vec2 n);

    std::vector<Chunk *> createChunks(OpenGLContext* context, int side, glm::vec3 pos);

    // Checks if a chunk has been created for the specified x,z world position
    bool chunkAt(int x, int z);

    BlockType getBlockAt(int x, int y, int z) const;   // Given a world-space coordinate (which may have negative
    // values) return the block stored at that point in space.
    void setBlockAt(int x, int y, int z, BlockType t); // Given a world-space coordinate (which may have negative
    // values) set the block at that point in space to the
    // given type.
    //rivers
    void setRivers(OpenGLContext* context);     //set up 2 rivers
    //void createChunksForRiver(OpenGLContext* context); //create chunks for rivers
    void createBlocksForRiver(OpenGLContext* context); //create blocks into terrain

    QString createPath(River* r); //draw one river in the stack
    QString processString(River* r, QString s); //expansion rule
    QString applyRules(River* r, QChar c); //apply rule

    void drawSys(River* r, QString s); //draw system
    void MoveAndDrawLine(River* r, Turtle* t, int x, int y); //for river system
    void MoveAndDrawLake(River* r, Turtle* t); //for lakes at the end of branches


    //caves

    void createEntrance();

    void setCaves(OpenGLContext* context);     //set up 2 rivers
    //void createChunksForCaves(OpenGLContext* context); //create chunks for rivers
    void createBlocksForCaves(OpenGLContext* context); //create blocks into terrain

    QString createPath(Cave* r); //draw one river in the stack
    QString processString(Cave* r, QString s); //expansion rule
    QString applyRules(Cave* r, QChar c); //apply rule

    void drawSys(Cave* r, QString s); //draw system
    void MoveAndDrawLine(Cave* r, Turtle* t, int x, int y); //for cave system
    void MoveAndDrawCavern(Cave* r, Turtle* t); //for caverns at the end of branches


    // Updates range of 4x4 chunks by calling create
    void updateChunks(int x, int z);
};


