/* source-heightmap.cpp
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

#include "source-heightmap.h"

OpenTerrain_DataSource_Heightmap::OpenTerrain_DataSource_Heightmap(OpenTerrain_Heightmap* _Heightmap)
{
//Set heightmap
	Heightmap = _Heightmap;
	
//Set flags
	IsWritable = false;
}

OpenTerrain_DataSource_Heightmap::~OpenTerrain_DataSource_Heightmap()
{
	
}

int OpenTerrain_DataSource_Heightmap::_GetChunk(float* Buffer, unsigned int Size, unsigned int X, unsigned int Y, unsigned int Step) const
{
//Check that the heightmap has data
	if(Heightmap->Points == 0)
		return -1;
		
//Check if the data is on the heightmap
	if(X < 0 || Y < 0)
		return 0;
	if(((X + 1) * Size * Step) > Heightmap->Width || ((Y + 1) * Size * Step) > Heightmap->Height)
		return 0;
		
//Copy data
	unsigned int XOrigin = Size * X * Step;
	unsigned int YOrigin = Size * Y * Step;
	for(unsigned int PX = 0; PX < Size + 1; PX++) {
		for(unsigned int PY = 0; PY < Size + 1; PY++) {
			Buffer[PY * (Size + 1) + PX] = Heightmap->Points[(YOrigin + PY * Step) * Heightmap->Width + (XOrigin + PX * Step)];
		}
	}
	
//Return success
	return 1;
}

int OpenTerrain_DataSource_Heightmap::_PutChunk(const float* Buffer, unsigned int Size, unsigned int X, unsigned int Y, unsigned int Step)
{
//Check that the heightmap has data
	if(Heightmap->Points == 0)
		return -1;
		
//Check if the data is on the heightmap
	if(X < 0 || Y < 0)
		return 0;
	if(((X + 1) * Size * Step) > Heightmap->Width || ((Y + 1) * Size * Step) > Heightmap->Height)
		return 0;
		
//Copy data
	unsigned int XOrigin = Size * X * Step;
	unsigned int YOrigin = Size * Y * Step;
	for(unsigned int PX = 0; PX < Size + 1; PX++) {
		for(unsigned int PY = 0; PY < Size + 1; PY++) {
			Heightmap->Points[(YOrigin + PY * Step) * Heightmap->Width + (XOrigin + PX * Step)] = Buffer[PY * (Size + 1) + PX];
		}
	}
	
//Return success
	return 1;
}
