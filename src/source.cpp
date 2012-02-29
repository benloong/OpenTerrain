/* source.cpp
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

#include "source.h"

int OpenTerrain_DataSource::GetChunk(float* Buffer, unsigned int Size, unsigned int X, unsigned int Y, int Stretch) const
{
//Work out step - Used for reducing detail
	unsigned int Step = 1;
	if(Stretch < 0 )
		Step = 1 << (-Stretch);
		
//Work out data source size - Used for increasing detail. As chunk size is constant, the higher the detail, the less area we should get from the data source
	unsigned int DataSourceSize = Size;
	if(Stretch > 0)
		DataSourceSize = Size >> Stretch;
		
//Pass on to data source
	return _GetChunk(Buffer, DataSourceSize, X, Y, Step);
	
//TODO: Expand chunk here (if Stretch > 0)
}

int OpenTerrain_DataSource::PutChunk(const float* Buffer, unsigned int Size, unsigned int X, unsigned int Y, int Stretch)
{
//Work out step - Used for reducing detail
	unsigned int Step = 1;
	if(Stretch < 0 )
		Step = 1 << (-Stretch);
		
//Work out data source size - Used for increasing detail. As chunk size is constant, the higher the detail, the less area we should get from the data source
	unsigned int DataSourceSize = Size;
	if(Stretch > 0)
		DataSourceSize = Size >> Stretch;
		
//TODO: Squash chunk here (if Stretch > 0)
		
//Pass on to data source
	return _PutChunk(Buffer, DataSourceSize, X, Y, Step);
}
