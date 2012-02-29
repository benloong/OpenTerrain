/* render-opengl.cpp
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

#include "render-opengl.h"
#include <SDL/SDL.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

struct OpenTerrain_OGL_ChunkDesc
{
	unsigned  int DisplayList;
	unsigned int VertexVBO;
	unsigned int IndexVBO;
	OpenTerrain_OGL_VertexPosition* Vertices;   //TEST
	unsigned int* Indices;   //TEST
};


OpenTerrain_Render_OpenGL::OpenTerrain_Render_OpenGL()
{
//Initialise glew
	glewInit();
}

OpenTerrain_Render_OpenGL::~OpenTerrain_Render_OpenGL()
{
	
}


void* OpenTerrain_Render_OpenGL::SetupChunk(OpenTerrain_Chunk* Chunk)
{
//Allocate chunkdesc
	OpenTerrain_OGL_ChunkDesc* ChunkDesc = new OpenTerrain_OGL_ChunkDesc;
	
//Initialise chunkdesc
	ChunkDesc->DisplayList = 0;
	ChunkDesc->VertexVBO = 0;
	
//Create display list
	//ChunkDesc->DisplayList = CreateDisplayList(Chunk);
	
//Create VBO
	//ChunkDesc->VertexVBO = CreateVertexVBO(Chunk);
	//ChunkDesc->IndexVBO = CreateVertexVBO(Chunk);
	
//Allocate vertices
	ChunkDesc->Vertices = new OpenTerrain_OGL_VertexPosition[Chunk->VertexCount];   //TEST
	
//Create vertex array
	CreateVertexArray(Chunk, ChunkDesc->Vertices);   //TEST
	
//Allocate indices
	ChunkDesc->Indices = new unsigned int[Chunk->QuadCount * 4];   //TEST
	
//Create index array
	CreateIndexArray(Chunk, ChunkDesc->Indices);   //TEST
	
//Return chunkdesc
	return (void*) ChunkDesc;
}


void OpenTerrain_Render_OpenGL::CreateVertexArray(OpenTerrain_Chunk* Chunk, OpenTerrain_OGL_VertexPosition* Vertices)   //TEST
{
//Get Terrain
	OpenTerrain_Terrain* Terrain = Chunk->LODLevel->Terrain;
	
//Calculate chunk position on screen
	int ChunkX = (Chunk->PosX - Terrain->ChunksX / 2) * Terrain->ChunkSize;
	int ChunkY = (Chunk->PosY - Terrain->ChunksY / 2) * Terrain->ChunkSize;
	
//Loop through vertices
	for(unsigned int Y = 0; Y < Terrain->ChunkSize + 1; Y++) {
		for(unsigned int X = 0; X < Terrain->ChunkSize + 1; X++) {
			Vertices[Y * (Terrain->ChunkSize + 1) + X].x = (ChunkX + X) * 4;
			Vertices[Y * (Terrain->ChunkSize + 1) + X].y = (ChunkY + Y) * 4;
			Vertices[Y * (Terrain->ChunkSize + 1) + X].z = Chunk->Vertices[Y * (Terrain->ChunkSize + 1) + X] * 100.0;
		}
	}
}


void OpenTerrain_Render_OpenGL::CreateIndexArray(OpenTerrain_Chunk* Chunk, unsigned int* Indices)   //TEST
{
//Get Terrain
	OpenTerrain_Terrain* Terrain = Chunk->LODLevel->Terrain;
	
//Calculate chunk position on screen
	int ChunkX = (Chunk->PosX - Terrain->ChunksX / 2) * Terrain->ChunkSize;
	int ChunkY = (Chunk->PosY - Terrain->ChunksY / 2) * Terrain->ChunkSize;
	
//Loop through quads
	for(unsigned int Y = 0; Y < Terrain->ChunkSize ; Y++) {
		for(unsigned int X = 0; X < Terrain->ChunkSize; X++) {
			//Indices[(Y * (Terrain->ChunkSize) + X) * 4 + 0] = Chunk->Quads[Y * (Terrain->ChunkSize) + X ].Vertices[0];
			//Indices[(Y * (Terrain->ChunkSize) + X) * 4 + 1] = Chunk->Quads[Y * (Terrain->ChunkSize) + X ].Vertices[1];
			//Indices[(Y * (Terrain->ChunkSize) + X) * 4 + 2] = Chunk->Quads[Y * (Terrain->ChunkSize) + X ].Vertices[2];
			//Indices[(Y * (Terrain->ChunkSize) + X) * 4 + 3] = Chunk->Quads[Y * (Terrain->ChunkSize) + X ].Vertices[3];
			//Indices[(Y * (Terrain->ChunkSize) + X) * 4 + 0]
			
		}
	}
}


unsigned int OpenTerrain_Render_OpenGL::CreateDisplayList(OpenTerrain_Chunk* Chunk)
{
//Create display list
	unsigned int DisplayList = glGenLists(1);

//Get Terrain
	OpenTerrain_Terrain* Terrain = Chunk->LODLevel->Terrain;

//Calculate chunk position on screen
	int ChunkX = (Chunk->PosX - Terrain->ChunksX / 2) * Terrain->ChunkSize;
	int ChunkY = (Chunk->PosY - Terrain->ChunksY / 2) * Terrain->ChunkSize;

//Start list
	glNewList(DisplayList, GL_COMPILE);
	glBegin(GL_QUADS);

//Loop through quads
	for(unsigned int Y = 0; Y < Terrain->ChunkSize; Y++) {
		for(unsigned int X = 0; X < Terrain->ChunkSize; X++) {
		//Draw quad
			float a = Chunk->Vertices[(Y + 0) * (Terrain->ChunkSize + 1) + X + 0];
			float b = Chunk->Vertices[(Y + 1) * (Terrain->ChunkSize + 1) + X + 0];
			float c = Chunk->Vertices[(Y + 1) * (Terrain->ChunkSize + 1) + X + 1];
			float d = Chunk->Vertices[(Y + 0) * (Terrain->ChunkSize + 1) + X + 1];
			int QuadX = (ChunkX + X) * 4;
			int QuadY = (ChunkY + Y) * 4;
			glColor3f(a, a, a); glVertex3f(QuadX, QuadY, a * 100);
			glColor3f(b, b, b); glVertex3f(QuadX, QuadY + 4, b * 100);
			glColor3f(c, c, c); glVertex3f(QuadX + 4, QuadY + 4, c * 100);
			glColor3f(d, d, d); glVertex3f(QuadX + 4, QuadY, d * 100);
		}
	}

//Finish list
	glEnd();
	glEndList();

//Return display list number
	return DisplayList;
}


int OpenTerrain_Render_OpenGL::RenderChunk(OpenTerrain_Chunk* Chunk)
{
//Get chunkdesc
	OpenTerrain_OGL_ChunkDesc* ChunkDesc = (OpenTerrain_OGL_ChunkDesc*)Chunk->RenderDriverData;

//Try drawing from display list
	if(ChunkDesc->DisplayList) {
		glCallList(ChunkDesc->DisplayList);
		return 0;
	}

//Return fail
	return -1;
}
