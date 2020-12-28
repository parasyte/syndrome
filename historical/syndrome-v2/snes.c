/*
  SNES routines for level editors, etc.
  Copyright 2002-2004 Parasyte
*/

#include "snes.h"


void snesDecodePal(u32 *out, u16 *in) {
	int i,p=0;

	for (i = 0; i < 128; i++) {
		out[p++] = pRGB((in[i] & 0x1F) << 3,((in[i] >> 5) & 0x1F) << 3,(in[i] >> 10) << 3);
	}
	for (i = 0; i < 128; i+=16) out[i] = out[0];
}

int snesRender(u32 *destbmp, u32 dx, u32 dy, u32 dw, u32 dh, TILEMAP *tilemap, u32 sx, u32 sy, u32 trans) {
	u32 oldsx=sx,oldsy=sy;
	u16 tile;
	u8 pal,tmp,tmptile[64],fliptile[64];
	int offset,x,y,p,i,j;

	if ((!tilemap->map) || (!tilemap->tiles)) return 1;
	if ((sx > tilemap->w) || (sy > tilemap->h)) return 0; //NO ERROR!

	destbmp += ((dh-1)*dw);
	destbmp += dx;
	destbmp -= (dy*dw);

	for (y = dy; y < dh; y+=8) {
		if (y >= ((tilemap->h - oldsy) << 3)) break;
		for (x = dx; x < dw; x+=8) {
			if (x >= ((tilemap->w - oldsx) << 3)) break;

			//decode tile
			tile = tilemap->map[sx+(sy*tilemap->w)];
			offset = (tile&0x3FF)*0x20;
			pal = ((tile >> 10) & 0x07);
			p=0;
			for (i = 0; i < 16; i+=2) {
				for (j = 7; j >= 0; j--) {
					tmp = (tilemap->tiles[offset+i] >> j) & 1;
					tmp |= ((tilemap->tiles[offset+i+0x01] >> j) & 1) << 1;
					tmp |= ((tilemap->tiles[offset+i+0x10] >> j) & 1) << 2;
					tmp |= ((tilemap->tiles[offset+i+0x11] >> j) & 1) << 3;
					tmp |= (pal << 4);
					tmptile[p++] = tmp;
				}
			}

			//flip the generated tile, if needed
			if ((tile >> 14) & 1) {
				p=0;
				for (i = 0; i < 8; i++) {
					for (j = 7; j >= 0; j--) {
						fliptile[p++] = tmptile[j+(i*8)];
					}
				}
				memcpy(tmptile,fliptile,64);
			}
			if (tile >> 15) {
				p=0;
				for (i = 7; i >= 0; i--) {
					for (j = 0; j < 8; j++) {
						fliptile[p++] = tmptile[j+(i*8)];
					}
				}
				memcpy(tmptile,fliptile,64);
			}

			//draw it!
			p=0;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					if ((!trans) || ((trans) && (tmptile[p] & 0x0F))) destbmp[(x+j)-((y+i)*dw)] = tilemap->pal[tmptile[p]];
					p++;
				}
			}

			sx++;
		}
		sx = oldsx;
		sy ++; //= (tilemap->w - (dw>>3));
	}

	return 0;
}

void snesDeleteTilemap(TILEMAP *tilemap) {
	if (tilemap->map) {
		free(tilemap->map);
		tilemap->map = 0;
	}
}
