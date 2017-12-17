#include "chunker.h"

Chunker::Chunker(QMutex *mutex, Terrain *terrain, Chunk *chunk)
    : mutex(mutex), terrain(terrain), chunk(chunk)
{}

void Chunker::run() {
    mutex->lock();
    // now set up VBO data and push to Chunk member variables instead of binding
    std::vector<GLuint> indices;
    // positions, normals, UVs
    std::vector<float> data;
    std::vector<float> dataTrans;

    int startIdx = 0;

    int origX = chunk->origin.first;
    int origZ = chunk->origin.second;

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

                BlockType block = chunk->getBlockAt(x, y, z);
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
                    neighbor = chunk->getBlockAt(x, y, z + 1);
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
                        neighbor = DIRT;
                    }
                    else {
                        neighbor = terrain->getBlockAt(xWorld - 1, y, zWorld);
                    }
                }
                else {
                    neighbor = chunk->getBlockAt(x - 1, y, z);
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
                        neighbor = DIRT;
                    }
                    else {
                        neighbor = terrain->getBlockAt(xWorld, y, zWorld - 1);
                    }
                }
                else {
                    neighbor = chunk->getBlockAt(x, y, z - 1);
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
                        neighbor = DIRT;
                    }
                    else {
                        neighbor = terrain->getBlockAt(xWorld + 1, y, zWorld);
                    }
                }
                else {
                    neighbor = chunk->getBlockAt(x + 1, y, z);
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
                    neighbor = DIRT;
                }
                else {
                    neighbor = chunk->getBlockAt(x, y + 1, z);
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
                    neighbor = chunk->getBlockAt(z, y - 1, z);
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
    mutex->unlock();

    mutex->lock();
    chunk->indices = indices;
    chunk->data = data;
    chunk->dataTrans = dataTrans;

    mutex->unlock();
}
