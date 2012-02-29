/* heightmap.cpp
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

#include "heightmap.h"
#include <SDL/SDL_image.h>

OpenTerrain_Heightmap::OpenTerrain_Heightmap()
{
//Initialise values
	Width = 0;
	Height = 0;
	Points = 0;
}

OpenTerrain_Heightmap::~OpenTerrain_Heightmap()
{
	
}

int OpenTerrain_Heightmap::CreateBlank(unsigned int _Width, unsigned int _Height)
{
//Check that the Heightmap is blank
	if(Points != 0)
		return -1;
		
//Create heightmap
	Width = _Width;
	Height = _Height;
	Points = new float[Width * Height];
}

int OpenTerrain_Heightmap::LoadFromImage(const char* FileName)
{
//Check that the Heightmap is blank
	if(Points != 0)
		return -1;
		
//Load image
	SDL_Surface* File = IMG_Load(FileName);
	if(File == 0)
		return -1;
		
//Create heightmap
	CreateBlank(File->w, File->h);
	
//Convert image
	for(unsigned int X = 0; X < Width; X++) {
		for(unsigned int Y = 0; Y < Height; Y++) {
			unsigned char R, G, B;
			unsigned int Pixel;
			
		//Get pixel
			//FROM: http://forum.gpwiki.org/viewtopic.php?t=4605
			int bpp = File->format->BytesPerPixel;
			unsigned char *p = (unsigned char *)File->pixels + Y * File->pitch + X * bpp;
			switch(bpp) {
				case 1:
					Pixel = *p;
				break;
				case 2:
					Pixel = *(unsigned short *)p;
				break;
				case 3:
					if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
						Pixel = p[0] << 16 | p[1] << 8 | p[2];
					else
						Pixel = p[0] | p[1] << 8 | p[2] << 16;
				break;
				case 4:
					Pixel = *(unsigned int *)p;
				break;
				default:
					Pixel = 0;
				break;
			}
			
			
		//Convert pixel into RGB
			SDL_GetRGB(Pixel, File->format, &R, &G, &B);
			
		//Put pixel into heightmap
			Points[Y * Width + X] = R / 255.0f;
		}
	}
	
//Close file
	SDL_FreeSurface(File);
}
