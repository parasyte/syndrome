/*
  SNES routines for level editors, etc.
  Copyright 2002-2004 Parasyte
*/

#ifndef _SNES_H_
#define _SNES_H_


#include "syndrome.h"


#define pRGB(a,b,c) (((a)<<16)|((b)<<8)|c)

typedef struct _TILEMAP {
	u16 *map;
//	u16 *bgmap; //FIXME: background and foreground should be combined into a single tilemap
//	u16 *fgmap;
	u32 w;
	u32 h;
	u32 pal[128]; //FIXME: add extra dimensions for palette swaps?
	u8 tiles[0x8000];
} TILEMAP;


//drawing routines
void snesDecodePal(u32 *out, u16 *in);
int snesRender(u32 *destbmp, u32 dx, u32 dy, u32 dw, u32 dh, TILEMAP *tilemap, u32 sx, u32 sy, u32 trans);
void snesDeleteTilemap(TILEMAP *tilemap);


#endif //_SNES_H_
