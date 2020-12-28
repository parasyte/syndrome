/*
  Syndrome - MegaMan 7 (SNES) Editor
  Copyright 2002-2004 Parasyte
*/

#ifndef _GUI_H_
#define _GUI_H_


#include <windows.h>
#include <stdio.h>
#include <string.h>


typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

//constants for virtual screen
#define BITMAPWIDTH		brect.right
#define BITMAPHEIGHT	brect.bottom
#define BITMAPBITWIDTH	BITMAPWIDTH*4

//virtual tile size in pixels, used for scrolling, must be a power of 8
#define TILESIZE	32
#define TILEMASK	(TILESIZE-1)

//define DEBUGGY to build debug version
#define DEBUGGY


extern u8 *filedata;
extern int filesize;
extern u32 *mainbmp;
//extern int mouse_x,mouse_y;
extern int ZOOM;
extern int levelwidth,levelheight;
extern int xlevelmax;
extern int ylevelmax;
extern int xscrollpos;
extern int yscrollpos;
extern int xscrollmax;
extern int yscrollmax;
extern int viewmode;


//win32 stuffs
extern char *FrameClassName;
extern char *MainClassName;

extern HWND Framehwnd,MDIClienthwnd;
extern HWND Mainhwnd;
extern HINSTANCE ghInstance;

extern HDC BitmapDC;
extern HBITMAP BitmapDIB;
extern RECT brect;

#endif //_GUI_H_
