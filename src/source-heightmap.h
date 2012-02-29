/* source-heightmap.h
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

#ifndef OPENTERRAIN_SOURCE_HEIGHTMAP_H
#define OPENTERRAIN_SOURCE_HEIGHTMAP_H

class OpenTerrain_DataSource_Heightmap;

#include "source.h"
#include "heightmap.h"


class OpenTerrain_DataSource_Heightmap: public OpenTerrain_DataSource
{
public:
	OpenTerrain_DataSource_Heightmap(OpenTerrain_Heightmap* Heightmap);
	~OpenTerrain_DataSource_Heightmap();
	
	int _GetChunk(float* Buffer, unsigned int Size, unsigned int X, unsigned int Y, unsigned int Step) const;
	int _PutChunk(const float* Buffer, unsigned int Size, unsigned int X, unsigned int Y, unsigned int Step);
	
private:
	OpenTerrain_Heightmap* Heightmap;
};

#endif
