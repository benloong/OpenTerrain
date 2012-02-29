/* terrain.cpp
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

#include "terrain.h"

OpenTerrain_Terrain::OpenTerrain_Terrain()
{
//Initialise values
	ChunkSize = 0;
	ChunksX = 0;
	ChunksY = 0;
	LODLevelCount = 0;
	SharedQuads = false;
	QuadRotation = false;
	QuadCount = 0;
	OpenTerrain_Quad* Quads;
	LODLevels = 0;
	DataSource = 0;
	RenderDriver = 0;
	
//Initialise setup values
	SetupValues.ChunkSize = 32;
	SetupValues.ChunksX = 8;
	SetupValues.ChunksY = 8;
	SetupValues.LODLevelCount = 1;
	SetupValues.SharedQuads = true;
	SetupValues.QuadRotation = false;
}

OpenTerrain_Terrain::~OpenTerrain_Terrain()
{
//Delete arrays if they exist
	if(LODLevels != 0)
		delete[] LODLevels;
}

int OpenTerrain_Terrain::Setup(OpenTerrain_DataSource* _DataSource, OpenTerrain_RenderDriver* _RenderDriver)
{
//Set values
	ChunkSize = SetupValues.ChunkSize;
	ChunksX =  SetupValues.ChunksX;
	ChunksY =  SetupValues.ChunksY;
	LODLevelCount =  SetupValues.LODLevelCount;
	SharedQuads = SetupValues.SharedQuads;
	QuadRotation = SetupValues.QuadRotation;
	DataSource = _DataSource;
	RenderDriver = _RenderDriver;
	
//Check values
	if(ChunksX == 0 || ChunksY == 0 || ChunkSize == 0 || LODLevelCount == 0)
		return 0;
	if((ChunkSize & (ChunkSize - 1)) != 0)
		return -1;
	if((ChunksX & (ChunksX - 1)) != 0)
		return -1;
	if((ChunksY & (ChunksY - 1)) != 0)
		return -1;
	unsigned int MinChunks = 1 << (LODLevelCount-1);
	if(ChunksX < MinChunks || ChunksY < MinChunks)
		return -1;
		
//Quads cannot be shared with quad rotation enabled
	if(QuadRotation = true)
		SharedQuads = false;
		
//Allocate quads
	if(SharedQuads == true) {
		QuadCount = (ChunkSize) * (ChunkSize);
		Quads = new OpenTerrain_Quad[QuadCount];
		OpenTerrain_Terrain::GenerateQuads(ChunkSize, Quads);
	}
	
//Allocate LODLevels
	LODLevels = new OpenTerrain_LODLevel[LODLevelCount];
	
//Setup LODLevels
	unsigned int LevelPO2 = 1;
	for(unsigned int Level = 0; Level < LODLevelCount; Level++) {
	//Setup LOD Level
		if(LODLevels[Level].Setup(this, Level, LevelPO2) == -1)
			return -1;
			
	//Calculate next PO2
		LevelPO2 *= 2;
	}
}


void OpenTerrain_Terrain::GenerateQuads(unsigned int Size, OpenTerrain_Quad* Buffer)
{
//Loop through quads
	for(unsigned int X = 0; X < Size; X++) {
		for(unsigned int Y = 0; Y < Size; Y++) {
			Buffer[Y * Size + X].Size = 1;
			Buffer[Y * Size + X].Rotation = false;
			Buffer[Y * Size + X].Vertices[0] = Y * Size + X;
			Buffer[Y * Size + X].Vertices[1] = Y * Size + X + 1;
			Buffer[Y * Size + X].Vertices[2] = (Y + 1) * Size + X;
			Buffer[Y * Size + X].Vertices[3] = (Y + 1) * Size + X + 1;
		}
	}
}

void OpenTerrain_Terrain::CalculateQuadRotation(unsigned int Size, OpenTerrain_Quad* Buffer, const float* Vertices)
{
//Loop through quads
	for(unsigned int X = 0; X < Size; X++) {
		for(unsigned int Y = 0; Y < Size; Y++) {
		//Get Quad
			OpenTerrain_Quad* Quad = &Buffer[Y * Size + X];
			
		//Get difference between top left and bottom right points
			float TLBRDiff = Vertices[Quad->Vertices[0]] - Vertices[Quad->Vertices[3]];
			if(TLBRDiff < 0)
				TLBRDiff = -TLBRDiff;
				
		//Get difference between top right and bottom left points
			float TRBLDiff = Vertices[Quad->Vertices[1]] - Vertices[Quad->Vertices[2]];
			if(TRBLDiff < 0)
				TRBLDiff = -TRBLDiff;
				
		//If TLBR is greater than TRBL then rotate the quad
			if(TRBLDiff < TLBRDiff)
				Quad->Rotation = true;
		}
	}
}



OpenTerrain_LODLevel::OpenTerrain_LODLevel()
{
//Initialise values
	Level = 0;
	ChunksX = 0;
	ChunksY = 0;
	LevelPO2 = 0;
	ChunkCount = 0;
	Chunks = 0;
	Terrain = 0;
}

OpenTerrain_LODLevel::~OpenTerrain_LODLevel()
{
//Delete arrays if they exist
	if(Chunks != 0)
		delete[] Chunks;
}

int OpenTerrain_LODLevel::Setup(OpenTerrain_Terrain* _Terrain, unsigned int _Level, unsigned int _LevelPO2)
{
//Set values
	Terrain = _Terrain;
	Level = _Level;
	LevelPO2 = _LevelPO2;
	ChunksX = Terrain->ChunksX / LevelPO2;
	ChunksY = Terrain->ChunksY / LevelPO2;
	ChunkCount = ChunksX * ChunksY;
	
//Allocate Chunks
	Chunks = new OpenTerrain_Chunk[ChunkCount];
	
//Setup Chunks
	for(unsigned int ChunkX = 0; ChunkX < ChunksX; ChunkX++) {
		for(unsigned int ChunkY = 0; ChunkY < ChunksY; ChunkY++) {
		//Setup Chunk
			if(Chunks[ChunkY * ChunksX + ChunkX].Setup(this, ChunkX, ChunkY) == -1)
				return -1;
		}
	}
}



OpenTerrain_Chunk::OpenTerrain_Chunk()
{
//Initialise values
	PosX = 0;
	PosY = 0;
	LODLevel = 0;
	Vertices = 0;
	Quads = 0;
	VertexCount = 0;
	QuadCount = 0;
	RenderDriverData = 0;
}

OpenTerrain_Chunk::~OpenTerrain_Chunk()
{
//Delete arrays if they exist
	if(Vertices != 0)
		delete[] Vertices;
		
	if(Quads != 0)
		delete[] Quads;
}

int OpenTerrain_Chunk::Setup(OpenTerrain_LODLevel* _LODLevel, unsigned int _PosX, unsigned int _PosY)
{
//Set values
	LODLevel = _LODLevel;
	PosX = _PosX;
	PosY = _PosY;
	
//Calculate vertex and quad count
	unsigned int ChunkSize = LODLevel->Terrain->ChunkSize;
	VertexCount = (ChunkSize + 1) * (ChunkSize + 1);
	
//Allocate vertices
	Vertices = new float[VertexCount];
	
//Allocate quads
	if(LODLevel->Terrain->SharedQuads == false) {
		QuadCount = (ChunkSize) * (ChunkSize);
		Quads = new OpenTerrain_Quad[QuadCount];
		OpenTerrain_Terrain::GenerateQuads(ChunkSize, Quads);
	} else {
		QuadCount = LODLevel->Terrain->QuadCount;
	}
	
//Get vertex data
	LODLevel->Terrain->DataSource->GetChunk(Vertices, ChunkSize, PosX, PosY, -LODLevel->Level);
	
//Setup on render driver
	if(LODLevel->Terrain->RenderDriver != 0)
		RenderDriverData = LODLevel->Terrain->RenderDriver->SetupChunk(this);
}