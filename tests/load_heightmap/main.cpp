#include <openterrain/terrain.h>
#include <openterrain/heightmap.h>
#include <openterrain/source-heightmap.h>
#include <openterrain/render-opengl.h>
#include <iostream>
#include <SDL/SDL.h>

void LockScreen(SDL_Surface *screen);
void UnlockScreen(SDL_Surface *screen);
void DrawPixel(SDL_Surface *screen, unsigned int x, unsigned int y, Uint8 R, Uint8 G, Uint8 B);

int main(int argc, char **argv)
{
//Initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	
//Setup screen
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	
//Load Terrain
	OpenTerrain_Heightmap MyHeightmap;
	MyHeightmap.LoadFromImage("heightmap.png");
	OpenTerrain_DataSource_Heightmap DataSource(&MyHeightmap);
	OpenTerrain_Terrain MyTerrain;
	
	MyTerrain.SetupValues.ChunkSize = 32;
	MyTerrain.SetupValues.ChunksX = 8;
	MyTerrain.SetupValues.ChunksY = 8;
	MyTerrain.SetupValues.LODLevelCount = 1;
	MyTerrain.Setup(&DataSource, 0);
	
//Lock Screen
	LockScreen(screen);
	
//Draw terrain
	unsigned int ChunkVerticesAccross = MyTerrain.ChunkSize + 1;
	for(unsigned int Chunk = 0; Chunk < MyTerrain.LODLevels[0].ChunkCount; Chunk++) {
	//Get chunk
		OpenTerrain_Chunk* ChunkPtr = &MyTerrain.LODLevels[0].Chunks[Chunk];
		
	//Calculate chunk position on screen
		unsigned int ChunkX = ChunkPtr->PosX * ChunkVerticesAccross;
		unsigned int ChunkY = ChunkPtr->PosY * ChunkVerticesAccross;
		
	//Loop through vertices
		for(unsigned int Y = 0; Y < ChunkVerticesAccross; Y++) {
			for(unsigned int X = 0; X < ChunkVerticesAccross; X++) {
			//Get luminance and draw pixel
				unsigned char Luminance = ChunkPtr->Vertices[Y * ChunkVerticesAccross + X] * 255;
				DrawPixel(screen, ChunkX + X, ChunkY + Y, Luminance, Luminance, Luminance);
			}
		}
	}
	
//Unlock screen and update
	UnlockScreen(screen);
	SDL_UpdateRect(screen, 0, 0, 256, 256);
	
//Event loop
    SDL_Event event;
	while(true) {
		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
				break;
			}
		}
	}
	
//Return 0
	return 0;
}



// ============================================= DRAWING FUNCTIONS

void LockScreen(SDL_Surface *screen)
{
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
}

void UnlockScreen(SDL_Surface *screen)
{
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
}

void DrawPixel(SDL_Surface *screen, unsigned int x, unsigned int y, Uint8 R, Uint8 G, Uint8 B)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);

    switch (screen->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* Probably 15-bpp or 16-bpp */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* Slow 24-bpp mode, usually not used */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *(bufp+screen->format->Rshift/8) = R;
            *(bufp+screen->format->Gshift/8) = G;
            *(bufp+screen->format->Bshift/8) = B;
        }
        break;

        case 4: { /* Probably 32-bpp */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
}
