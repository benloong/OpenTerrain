/* heightmap.h
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

#ifndef OPENTERRAIN_HEIGHTMAP_H
#define OPENTERRAIN_HEIGHTMAP_H

class OpenTerrain_Heightmap
{
public:
	unsigned int Width, Height;
	float* Points;

	OpenTerrain_Heightmap();
	~OpenTerrain_Heightmap();
	
	int CreateBlank(unsigned int Width, unsigned int Height);
	int LoadFromImage(const char* FileName);
};

#endif
