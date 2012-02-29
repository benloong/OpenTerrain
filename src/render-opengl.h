/* render-opengl.h
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

#ifndef OPENTERRAIN_RENDER_OPENGL_H
#define OPENTERRAIN_RENDER_OPENGL_H

class OpenTerrain_RenderDriver_OpenGL;

#include "render.h"

struct OpenTerrain_OGL_VertexPosition   //TEST
{
	float x, y, z;
};

class OpenTerrain_Render_OpenGL: public OpenTerrain_RenderDriver
{
public:
	OpenTerrain_Render_OpenGL();
	~OpenTerrain_Render_OpenGL();
	
	void* SetupChunk(OpenTerrain_Chunk* Chunk);
	int RenderChunk(OpenTerrain_Chunk* Chunk);
	
	void CreateVertexArray(OpenTerrain_Chunk* Chunk, OpenTerrain_OGL_VertexPosition* Vertices);   //TEST
	void CreateIndexArray(OpenTerrain_Chunk* Chunk, unsigned int* Indices);   //TEST
	
	unsigned int CreateVertexVBO(OpenTerrain_Chunk* Chunk);
	unsigned int CreateIndexVBO(OpenTerrain_Chunk* Chunk);
	unsigned int CreateDisplayList(OpenTerrain_Chunk* Chunk);
	
};

#endif
