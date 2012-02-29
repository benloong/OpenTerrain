/* terrain.h
  Copyright (C) 2012 Karl Hobley

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Karl Hobley <karlh@openterrain.org>

*/

#ifndef OPENTERRAIN_TERRAIN_H
#define OPENTERRAIN_TERRAIN_H

class OpenTerrain_Terrain;
class OpenTerrain_LODLevel;
class OpenTerrain_Chunk;
struct OpenTerrain_Quad;

#include "render.h"
#include "source.h"

struct OpenTerrain_TerrainSetup
{
	unsigned int ChunkSize;
	unsigned int ChunksX, ChunksY;
	unsigned int LODLevelCount;
	bool SharedQuads;
	bool QuadRotation;
};

class OpenTerrain_Terrain
{
public:
	OpenTerrain_TerrainSetup SetupValues;
	
	unsigned int ChunkSize;
	unsigned int ChunksX, ChunksY;
	unsigned int LODLevelCount;
	bool SharedQuads;
	bool QuadRotation;
	unsigned int QuadCount;
	OpenTerrain_Quad* Quads;
	OpenTerrain_LODLevel* LODLevels;
	OpenTerrain_DataSource* DataSource;
	OpenTerrain_RenderDriver* RenderDriver;
	
	OpenTerrain_Terrain();
	~OpenTerrain_Terrain();
	int Setup(OpenTerrain_DataSource* DataSource, OpenTerrain_RenderDriver* _RenderDriver);
	
	static void GenerateQuads(unsigned int Size, OpenTerrain_Quad* Buffer);
	static void CalculateQuadRotation(unsigned int Size, OpenTerrain_Quad* Buffer, const float* Vertices);
};

class OpenTerrain_LODLevel
{
public:
	unsigned int Level;
	unsigned int ChunksX, ChunksY;
	unsigned int LevelPO2;
	unsigned int ChunkCount;
	OpenTerrain_Chunk* Chunks;
	OpenTerrain_Terrain* Terrain;
	
	OpenTerrain_LODLevel();
	~OpenTerrain_LODLevel();
	int Setup(OpenTerrain_Terrain* Terrain, unsigned int Level, unsigned int LevelPO2);
	
	void RotateChunks(int X, int Y);
};

class OpenTerrain_Chunk
{
public:
	unsigned int PosX, PosY;
	OpenTerrain_LODLevel* LODLevel;
	float* Vertices;
	OpenTerrain_Quad* Quads;
	unsigned int VertexCount;
	unsigned int QuadCount;
	void* RenderDriverData;
	
	OpenTerrain_Chunk();
	~OpenTerrain_Chunk();
	int Setup(OpenTerrain_LODLevel* LODLevel, unsigned int PosX, unsigned int PosY);
};

struct OpenTerrain_Quad
{
	unsigned char Size;
	bool Rotation;
	int Vertices[4];
};

#endif
