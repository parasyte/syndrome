/*
  Syndrome - MegaMan 7 (SNES) Editor
  Copyright 2002-2004 Parasyte
*/

#ifndef _MM7_H_
#define _MM7_H_


#include "syndrome.h"
#include "snes.h"


extern int mm7Stage;


//MegaMan 7 routines
void mm7LoadPal(u16 *pal, u16 index);
int mm7LoadStagePal(u16 *pal, u8 palswap, u8 stage);
int mm7BuildStage(TILEMAP *fgmap, TILEMAP *bgmap, u8 stage);
void mm7BuildRoom(TILEMAP *tilemap, u32 x, u32 y, u16 room);
void mm7BuildStruct(TILEMAP *tilemap, u32 x, u32 y, u16 structure);
void mm7BuildBlock(TILEMAP *tilemap, u32 x, u32 y, u16 block);
int mm7decmp(u8 *decmp, u32 size, u16 index);

#endif //_MM7_H_
