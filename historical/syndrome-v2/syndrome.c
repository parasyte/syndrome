/*
  Syndrome - MegaMan 7 (SNES) Editor
  Copyright 2002-2004 Parasyte
*/

#include "syndrome.h"

#include "snes.h"
#include "mm7.h"


//"local globals"
#define VERSION "v0.1 *PRIVATE*"

#ifdef DEBUGGY
#define FRAMETITLE "Syndrome - "VERSION" [Debug Build]"
#else //DEBUGGY
#define FRAMETITLE "Syndrome - "VERSION
#endif //DEBUGGY

#define LWIDTH 512
#define LHEIGHT 320

TILEMAP stagefg,stagebg;


//globals
u8 *filedata;
int filesize;
u32 *mainbmp;
//int mouse_x,mouse_y;
int ZOOM=0;
int levelwidth=LWIDTH,levelheight=LHEIGHT;
int xlevelmax=(LWIDTH/TILESIZE);
int ylevelmax=(LHEIGHT/TILESIZE);
int xscrollpos=0;
int yscrollpos=0;
int xscrollmax=0;
int yscrollmax=0;
int viewmode=0;


//win32 stuffs
char *FrameClassName = "GUIFRAMECLASS";
char *MainClassName = "GUIMAINCLASS";

HWND Framehwnd,MDIClienthwnd;
HWND Mainhwnd = NULL;
HINSTANCE ghInstance;

HDC BitmapDC=NULL;
HBITMAP BitmapDIB=NULL;
RECT brect;



void guiBuildStage(int stage) {
	LARGE_INTEGER time,time2;

#ifdef DEBUGGY
	printf("Decoding stage %d...\n",stage);
	QueryPerformanceCounter(&time);
#endif //DEBUGGY

	snesDeleteTilemap(&stagefg);
	snesDeleteTilemap(&stagebg);
	if (mm7BuildStage(&stagefg,&stagebg,stage)) {
		MessageBox(Framehwnd, "Error loading stage", "ERROR", MB_ICONERROR | MB_OK);
		return;
	}
	levelwidth = (stagefg.w << 3); //(((stagefg.w + (stagebg.w * 2)) / 2) * 8);
	levelheight = (stagefg.h << 3); //(((stagefg.h + (stagebg.h * 2)) / 2) * 8);
	xlevelmax = (levelwidth / TILESIZE);
	ylevelmax = (levelheight / TILESIZE);
	xscrollpos = 0;
	yscrollpos = 0;

#ifdef DEBUGGY
	QueryPerformanceCounter(&time2);
	printf("Stage decoding completed in %#.3fms\n\n", (float)((time2.LowPart-time.LowPart)/(float)(1000)));
#endif //DEBUGGY
}

void guiRefresh(HDC hpaintdc) {
	if (ZOOM) StretchBlt(hpaintdc,0,0,(BITMAPWIDTH<<ZOOM),(BITMAPHEIGHT<<ZOOM),BitmapDC,0,0,BITMAPWIDTH,BITMAPHEIGHT,SRCCOPY);
	else BitBlt(hpaintdc,0,0,BITMAPWIDTH,BITMAPHEIGHT,BitmapDC,0,0,SRCCOPY);
}

void guiRedraw() {
	LARGE_INTEGER time,time2;

#ifdef DEBUGGY
	printf("Redrawing canvas...\n");
	QueryPerformanceCounter(&time);
#endif //DEBUGGY

	memset(mainbmp,0,(BITMAPHEIGHT*BITMAPBITWIDTH));
//	snesRender(mainbmp, 0,0, BITMAPWIDTH,BITMAPHEIGHT, &stagebg, ((xscrollpos>8)?((xscrollpos<<1)+16):(xscrollpos<<2)),(yscrollpos<<2), 0); //FIXME

	switch (viewmode) {
		case 0:
			snesRender(mainbmp, 0,0, BITMAPWIDTH,BITMAPHEIGHT, &stagefg, (xscrollpos<<2),(yscrollpos<<2), 1);
			break;
		case 1:
			snesRender(mainbmp, 0,0, BITMAPWIDTH,BITMAPHEIGHT, &stagebg, (xscrollpos<<2),(yscrollpos<<2), 1);
			break;
		case 2:
			snesRender(mainbmp, 0,0, BITMAPWIDTH,BITMAPHEIGHT, &stagebg, (xscrollpos<<2),(yscrollpos<<2), 0);
			snesRender(mainbmp, 0,0, BITMAPWIDTH,BITMAPHEIGHT, &stagefg, (xscrollpos<<2),(yscrollpos<<2), 1);
			break;
#ifdef DEBUGGY
		default:
			printf("\nWOAH! This should not happen!\n viewmode > 2\n\n");
			break;
#endif
	}

#ifdef DEBUGGY
	QueryPerformanceCounter(&time2);
	printf("Canvas redrawing completed in %#.3fms\n\n", (float)((time2.QuadPart-time.QuadPart)/(float)(1000)));
#endif //DEBUGGY
}

void guiCreateBMI(BITMAPINFO *bmi, int width, int height) {
	memset(&bmi->bmiHeader,0,sizeof(BITMAPINFOHEADER));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth = width;
	bmi->bmiHeader.biHeight = height;
	bmi->bmiHeader.biPlanes = 1;
	bmi->bmiHeader.biBitCount = 32;
	bmi->bmiHeader.biCompression = BI_RGB;
}

int guiDeleteBitmap(HDC *destDC, HBITMAP *destDIB) {
	if (*destDC) {
		DeleteDC(*destDC);
		*destDC = NULL;
	}
	if (*destDIB) {
		DeleteObject(*destDIB);
		*destDIB = NULL;
	}
	return 0;
}

int guiCreateBitmap(HWND hWnd, HDC *destDC, HBITMAP *destDIB, int width, int height, u32 **bmpbits) {
	BITMAPINFO bmInfo;
	HDC pDC;
	HBITMAP TmpBMP;

	guiCreateBMI(&bmInfo,width,height);

	if (!(*destDIB = CreateDIBSection((HDC)NULL,&bmInfo,DIB_RGB_COLORS,(void**)bmpbits,0,0))) {
		return 0;
	}

	pDC = GetDC(hWnd);
	*destDC = CreateCompatibleDC(pDC);
	ReleaseDC(hWnd,pDC);
	if (!*destDC) {
		return guiDeleteBitmap(destDC,destDIB);
	}

	if (!(TmpBMP = SelectObject(*destDC,*destDIB))) {
		return guiDeleteBitmap(destDC,destDIB);
	}
	DeleteObject(TmpBMP);

	return 1;
}

void UpdateScrollbarX(HWND hWnd) {
	SCROLLINFO si;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nMin = 0;
	si.nMax = xscrollmax;
	si.nPos = xscrollpos;
	si.nPage = 1;
	SetScrollInfo(hWnd,SB_HORZ,&si,TRUE);
}

void UpdateScrollbarY(HWND hWnd) {
	SCROLLINFO si;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nMin = 0;
	si.nMax = yscrollmax;
	si.nPos = yscrollpos;
	si.nPage = 1;
	SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
}

void UpdateBitmapSize(HWND hWnd, HDC *destDC, HBITMAP *destDIB, int x, int y) {
	//SCROLLINFO si;
	RECT crect;
	int doinvalidate=0;

	//align bitmap width and height to tile size
	crect.right = (((x/TILESIZE)*TILESIZE) >> ZOOM); //((x + TILEMASK) & ~TILEMASK) >> ZOOM;
	crect.bottom = (((y/TILESIZE)*TILESIZE) >> ZOOM); //((y + TILEMASK) & ~TILEMASK) >> ZOOM;

	//setup scrollbars
	xscrollmax = (xlevelmax-(crect.right/TILESIZE));
	if (xscrollmax <= 0) {
		xscrollmax = 0;
		xscrollpos = 0;
		doinvalidate = 1;
	}
	UpdateScrollbarX(hWnd);

	yscrollmax = (ylevelmax-(crect.bottom/TILESIZE));
	if (yscrollmax <= 0) {
		yscrollmax = 0;
		yscrollpos = 0;
		doinvalidate = 1;
	}
	UpdateScrollbarY(hWnd);

	//align bitmap, if required
	if (x > crect.right) crect.right += TILESIZE;
	if (y > crect.bottom) crect.bottom += TILESIZE;

	if ((crect.right) && (crect.bottom) && ((crect.right != BITMAPWIDTH) || (crect.bottom != BITMAPHEIGHT))) {
		BITMAPWIDTH = crect.right;
		BITMAPHEIGHT = crect.bottom;
		guiDeleteBitmap(destDC,destDIB);
		if (!guiCreateBitmap(hWnd,destDC,destDIB,BITMAPWIDTH,BITMAPHEIGHT,&mainbmp)) {
			MessageBox(Framehwnd, "Could not create main mainbmp", "ERROR", MB_ICONERROR | MB_OK);
			PostQuitMessage(0);
			return;
		}
		guiRedraw();
	}
	if (doinvalidate) {
		UpdateWindow(hWnd);
		InvalidateRect(hWnd,NULL,FALSE);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hpaintdc;
	PAINTSTRUCT ps;
	RECT crect;
	int tmp;

	switch (msg) {
		case WM_VSCROLL:
			switch(LOWORD(wParam)) {
				case SB_LINEUP: tmp = yscrollpos - 1; break;
				case SB_LINEDOWN: tmp = yscrollpos + 1; break;
				case SB_PAGEUP: tmp = yscrollpos - TILESIZE; break;
				case SB_PAGEDOWN: tmp = yscrollpos + TILESIZE; break;
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK: tmp = HIWORD(wParam); break;
				default: tmp = yscrollpos;
			}
			tmp = max(0,tmp);
			tmp = min(yscrollmax,tmp);
			if (tmp == yscrollpos) break;
			yscrollpos = tmp;
			UpdateScrollbarY(hWnd);

			guiRedraw(); //FIXME
			UpdateWindow(hWnd);
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_HSCROLL:
			switch(LOWORD(wParam)) {
				case SB_LINEUP: tmp = xscrollpos - 1; break;
				case SB_LINEDOWN: tmp = xscrollpos + 1; break;
				case SB_PAGEUP: tmp = xscrollpos - TILESIZE; break;
				case SB_PAGEDOWN: tmp = xscrollpos + TILESIZE; break;
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK: tmp = HIWORD(wParam); break;
				default: tmp = xscrollpos;
			}
			tmp = max(0,tmp);
			tmp = min(xscrollmax,tmp);
			if (tmp == xscrollpos) break;
			xscrollpos = tmp;
			UpdateScrollbarX(hWnd);

			guiRedraw(); //FIXME
			UpdateWindow(hWnd);
			InvalidateRect(hWnd,NULL,FALSE);
			break;
//		case WM_CREATE:
//			break;
		case WM_ERASEBKGND:
			return TRUE; //do not erase background
		case WM_PAINT:
			hpaintdc = BeginPaint(hWnd, &ps);
			guiRefresh(hpaintdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_SIZE:
			UpdateBitmapSize(hWnd, &BitmapDC, &BitmapDIB, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_KEYDOWN:
#ifdef DEBUGGY
			printf("WM_KEYDOWN: %d [Main]\n",wParam);
#endif //DEBUGGY
			switch (wParam) {
				case VK_PRIOR: //page up
					mm7Stage++;
					if (mm7Stage > 13) mm7Stage = 0;
					guiBuildStage(mm7Stage);
					GetClientRect(hWnd,&crect);
					UpdateBitmapSize(hWnd, &BitmapDC, &BitmapDIB, crect.right, crect.bottom);
					UpdateScrollbarX(hWnd);
					UpdateScrollbarY(hWnd);
					viewmode = 0;
					guiRedraw();
					UpdateWindow(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case VK_NEXT: //page down
					mm7Stage--;
					if (mm7Stage < 0) mm7Stage = 13;
					guiBuildStage(mm7Stage);
					GetClientRect(hWnd,&crect);
					UpdateBitmapSize(hWnd, &BitmapDC, &BitmapDIB, crect.right, crect.bottom);
					UpdateScrollbarX(hWnd);
					UpdateScrollbarY(hWnd);
					viewmode = 0;
					guiRedraw();
					UpdateWindow(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;
				case VK_TAB: //tab
					viewmode++;
					if (viewmode > 2) viewmode = 0;
					guiRedraw();
					UpdateWindow(hWnd);
					InvalidateRect(hWnd,NULL,FALSE);
					break;
			}
			break;
		case WM_CLOSE:
		case WM_QUIT:
			SendMessage(hWnd,WM_MDIDESTROY,wParam,lParam);
			break;
		case WM_DESTROY:
			guiDeleteBitmap(&BitmapDC,&BitmapDIB);
			free(filedata);
			snesDeleteTilemap(&stagefg);
			snesDeleteTilemap(&stagebg);
			EnableMenuItem(GetMenu(Framehwnd),801,MF_ENABLED); //open
			EnableMenuItem(GetMenu(Framehwnd),802,MF_GRAYED); //close
			break;
	}
	return DefMDIChildProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK FrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	CLIENTCREATESTRUCT ccs;
	MDICREATESTRUCT mcs;
	OPENFILENAME ofn;
	char fname[512],dname[512],str[512];
	FILE *fin;
	int tmp;

	switch (msg) {
		case WM_CREATE:
			ccs.hWindowMenu  = NULL;
			ccs.idFirstChild = 5001;

			MDIClienthwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "mdiclient", NULL,
				WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            	hWnd, (HMENU)5000, ghInstance, (LPVOID)&ccs);

			if (!MDIClienthwnd) {
				MessageBox(hWnd, "Could not create MDI client window", "ERROR", MB_ICONERROR | MB_OK);
				PostQuitMessage(0);
			}

            ShowWindow(MDIClienthwnd, TRUE);
			break;
		case WM_ERASEBKGND:
			return TRUE; //do not erase background
		case WM_COMMAND:
			switch (HIWORD(wParam)) {
				case BN_CLICKED:
					switch (LOWORD(wParam)) {
						case 801: //File -> Open
							//Open dialog, load file, etc here
							memset(&ofn,0,sizeof(ofn));
							fname[0] = '\0';
							GetCurrentDirectory(512,dname);

							ofn.lStructSize=sizeof(ofn);
							ofn.hwndOwner = hWnd;
							ofn.nMaxFile = 512;
							ofn.lpstrTitle = "Load ROM File...";
							ofn.lpstrFilter = "SNES ROM Files (*.smc)\0*.smc\0All files (*.*)\0*.*\0";
							ofn.lpstrFile = fname;
							ofn.lpstrInitialDir = dname;
							ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
							if (!GetOpenFileName(&ofn)) break;

							//open file, read data
							if (!(fin = fopen(fname,"rb"))) {
								MessageBox(hWnd, "Could not open ROM file", "ERROR", MB_ICONERROR | MB_OK);
								break;
							}
							fseek(fin,0,SEEK_END);
							filesize = ftell(fin);
							if (filesize < 0x10000) {
								MessageBox(hWnd, "File size error", "ERROR", MB_ICONERROR | MB_OK);
								break;
							}
							if (!(filedata = (u8*)malloc(filesize & 0xFFFF8000))) {
								MessageBox(hWnd, "Could not allocate memory for file data", "ERROR", MB_ICONERROR | MB_OK);
								break;
							}
							tmp = (filesize & 0x7FFF); //skip header, if present
							fseek(fin,tmp,SEEK_SET);
							fread(filedata,1,filesize-tmp,fin);
							fclose(fin);

							//verify rom file
							if (strncmp(filedata+0xFFC0,"MEGAMAN 7            ",0x15) != 0) {
								MessageBox(hWnd, "Invalid MegaMan 7 ROM", "ERROR", MB_ICONERROR | MB_OK);
								free(filedata);
								break;
							}
							if (filedata[0xFFD9] != 0x01) {
								MessageBox(hWnd, "This version of Syndrome only supports the USA MegaMan 7 ROM", "ERROR", MB_ICONERROR | MB_OK);
								free(filedata);
								break;
							}

							//build stage data
							viewmode = 0;
							memset(&stagefg,0,sizeof(TILEMAP));
							memset(&stagebg,0,sizeof(TILEMAP));
							guiBuildStage(0);

#ifdef DEBUGGY
							strcpy(str,dname);
							strcat(str,"\\map.bin");
							if ((fin = fopen(str,"wb"))) {
								fwrite(stagefg.map,1,(stagefg.w*stagefg.h*sizeof(u16)),fin);
								fclose(fin);
							}

							strcpy(str,dname);
							strcat(str,"\\tiles.bin");
							if ((fin = fopen(str,"wb"))) {
								fwrite(stagefg.tiles,1,0x8000,fin);
								fclose(fin);
							}
#endif //DEBUGGY

							//change menu items to "open" state
							EnableMenuItem(GetMenu(hWnd),801,MF_GRAYED); //open
							EnableMenuItem(GetMenu(hWnd),802,MF_ENABLED); //close

							//create main editor window
							mcs.szTitle = "Level Editor";
							mcs.szClass = MainClassName;
							mcs.hOwner  = ghInstance;
							mcs.x = CW_USEDEFAULT;
							mcs.y = CW_USEDEFAULT;
							mcs.cx = 380;
							mcs.cy = 360;
							mcs.style = MDIS_ALLCHILDSTYLES;

							if (!(Mainhwnd = (HWND)SendMessage(MDIClienthwnd,WM_MDICREATE,0,(LONG)&mcs))) {
								MessageBox(hWnd, "Could not create MDI child window", "ERROR", MB_ICONERROR | MB_OK);
								PostQuitMessage(0);
							}
							break;
						case 802: //File -> Close
							SendMessage(MDIClienthwnd,WM_MDIDESTROY,(WPARAM)Mainhwnd,0);
							break;
						case 803: //File -> Exit
							PostQuitMessage(0);
							break;
					}
					break;
			}
			//SendMessage(Mainhwnd, WM_COMMAND, wParam, lParam);
			break;
		case WM_KEYDOWN:
#ifdef DEBUGGY
			printf("WM_KEYDOWN: %d [Frame]\n",wParam);
#endif //DEBUGGY
			break;
		case WM_CLOSE:
		case WM_QUIT:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefFrameProc(hWnd, MDIClienthwnd, msg, wParam, lParam);
}

int CreateAllWindows(HINSTANCE hInstance) {
	WNDCLASSEX FrameWindow,MainWindow;
	HICON hIcon = LoadIcon(hInstance, "ICON");
	HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);

	memset(&FrameWindow,0,sizeof(WNDCLASSEX));
	FrameWindow.cbSize=sizeof(WNDCLASSEX);
	FrameWindow.style = 0;
	FrameWindow.lpfnWndProc = FrameProc;
	FrameWindow.cbClsExtra = 0;
	FrameWindow.cbWndExtra = 0;
	FrameWindow.hInstance = hInstance;
	FrameWindow.hIcon = hIcon;
	FrameWindow.hIconSm = hIcon;
	FrameWindow.hCursor = hCursor;
	FrameWindow.hbrBackground = (HBRUSH)COLOR_WINDOW;
	FrameWindow.lpszMenuName = "MAINMENU";
	FrameWindow.lpszClassName = FrameClassName;

	memset(&MainWindow,0,sizeof(WNDCLASSEX));
	MainWindow.cbSize=sizeof(WNDCLASSEX);
	MainWindow.style = 0;
	MainWindow.lpfnWndProc = WndProc;
	MainWindow.cbClsExtra = 0;
	MainWindow.cbWndExtra = 0;
	MainWindow.hInstance = hInstance;
	MainWindow.hIcon = hIcon;
	MainWindow.hIconSm = hIcon;
	MainWindow.hCursor = hCursor;
	MainWindow.hbrBackground = (HBRUSH)0;
	MainWindow.lpszMenuName = NULL;
	MainWindow.lpszClassName = MainClassName;

	if ((!RegisterClassEx(&FrameWindow)) || (!RegisterClassEx(&MainWindow))) {
		MessageBox(NULL, "Could not register window classes", "ERROR", MB_ICONERROR | MB_OK);
		return 0;
	}

	Framehwnd = CreateWindowEx(WS_EX_STATICEDGE,
		FrameClassName,
		FRAMETITLE,
		WS_OVERLAPPEDWINDOW, //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL);
	if (!Framehwnd) {
		MessageBox(NULL, "Could not create main window", "ERROR", MB_ICONERROR | MB_OK);
		return 0;
	}

	return 1;
}

void DeleteAllWindows(HINSTANCE hInstance) {
	UnregisterClass(FrameClassName, hInstance);
	UnregisterClass(MainClassName, hInstance);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
	MSG msg;

	ghInstance = hInstance;

	if (!CreateAllWindows(hInstance)) return 0;
	ShowWindow(Framehwnd,ncmdshow);
	UpdateWindow(Framehwnd);

	while (GetMessage(&msg,NULL,0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteAllWindows(hInstance);

	return msg.wParam;
}
