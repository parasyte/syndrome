// SyndromeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Syndrome.h"
#include "SyndromeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define DATA_SIZE    0x10000

#define APP_VERSION  "v0.1 Private"   //Program Version!
#define SCROLLBAR_W  16    //Scrollbar width!
#define SCREEN_X     6     //Where to draw your canvas on your dialog!  X,Y
#define SCREEN_Y     7
#define SCREEN_W     256   //size of display area!  Width, Height
#define SCREEN_H     256

#define CANVAS_W     4608  //8192   //Canvas size!  Width,Height
#define CANVAS_H     256
#define CANVAS_SIZE  (CANVAS_W*CANVAS_H)

bool FileLoaded = FALSE;
int iZoom = 1;
unsigned char canvas[CANVAS_SIZE], buffer[DATA_SIZE];
unsigned short TSAData[DATA_SIZE], StructData[DATA_SIZE], MapData[DATA_SIZE];
COLORREF canvasPal[128];

CString FileName;
FILE *fp;


unsigned char TileData[DATA_SIZE] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


void decode_pal();
void decmp_mm7(int decmpsize);
void decode_map(int mapnum, int x, int y);
void decode_struct(int structnum, int x, int y);
void decode_tsa(int tsanum, int x, int y);
void decode4bpp(int tilenum, int x, int y, char vflip, char hflip, char pal);
void plotpixel(int offset, int x, unsigned char c);


void decode_pal() {
	int pal,i,p=0;

	for (i = 0; i < 0x100; i+=2) {
		pal = buffer[i] | (buffer[i+1] << 8);
		canvasPal[p++] = RGB((pal & 0x1F) << 3,((pal >> 5) & 0x1F) << 3,(pal >> 10) << 3);
	}
	for (i = 0; i < 0x100; i+=16)
		canvasPal[i] = RGB(255,0,255); //background, pink
}

void decmp_mm7(int decmpsize) {
	unsigned char decmp[DATA_SIZE], ctrl, j;
	int addr = 0, count = 0, i, k;

	while (count < decmpsize) {
		ctrl = buffer[addr++];

		for (i = 0; i < 8; i++) {
			if (ctrl & 0x80) {
				j = buffer[addr] >> 2;
				k = ((buffer[addr] & 3) << 8) | buffer[addr+1];
				addr+=2;
				while (j != 0) {
					decmp[count++] = decmp[count-k];
					j--;
				}
				if (count >= decmpsize) break;
			}
			else {
				decmp[count++] = buffer[addr++];
				if (count >= decmpsize) break;
			}
			ctrl <<= 1;
		}
	}

	for (i = 0; i < decmpsize; i++)
		TileData[i+0x200] = decmp[i];
}

void decode_map(int mapnum, int x, int y) {
	int i,j;

	mapnum<<=6;
	x<<=3;
	y<<=3;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			decode_struct((MapData[mapnum++]>>3),(x+j),(y+i));
		}
	}
}

void decode_struct(int structnum, int x, int y) {
	int i,j;
	
	structnum<<=2;
	x<<=1;
	y<<=1;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			decode_tsa(StructData[structnum++],(x+j),(y+i));
		}
	}
}

void decode_tsa(int tsanum, int x, int y) {
	int tmp,i,j;

	tsanum<<=2;
	x<<=1;
	y<<=1;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			tmp = TSAData[tsanum++];
			decode4bpp((tmp&0x3FF),(x+j),(y+i),(tmp>>15),((tmp>>14)&1),((tmp>>10)&7));
		}
	}

}

void decode4bpp(int tilenum, int x, int y, char vflip, char hflip, char pal) {
	int offset,i,j,p=0;
	unsigned char tmp, tmptile[64], fliptile[64];

	offset = tilenum*0x20;
	for (i = 0; i < 16; i+=2) {
		for (j = 7; j >= 0; j--) {
			tmp = (TileData[offset+i] >> j) & 1;
			tmp |= ((TileData[offset+i+0x01] >> j) & 1) << 1;
			tmp |= ((TileData[offset+i+0x10] >> j) & 1) << 2;
			tmp |= ((TileData[offset+i+0x11] >> j) & 1) << 3;
			tmp |= (pal << 4);
			tmptile[p++] = tmp;
		}
	}


	//flip the generated tile, if needed
	if (hflip) {
		p=0;
		for (i = 0; i < 8; i++) {
			for (j = 7; j >= 0; j--) {
				fliptile[p++] = tmptile[j+(i*8)];
			}
		}
		for (i = 0; i < 64; i++)
			tmptile[i] = fliptile[i];
	}
	if (vflip) {
		p=0;
		for (i = 7; i >= 0; i--) {
			for (j = 0; j < 8; j++) {
				fliptile[p++] = tmptile[j+(i*8)];
			}
		}
		for (i = 0; i < 64; i++)
			tmptile[i] = fliptile[i];
	}


	//now draw the tile to our canvas
	x<<=3;
	y<<=3;
	p=0;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			plotpixel(j+x,i+y,tmptile[p++]);
		}
	}
}

void plotpixel(int x, int y, unsigned char c) {
	int pos = (x+(y*CANVAS_W));

	canvas[pos] = c;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyndromeDlg dialog

CSyndromeDlg::CSyndromeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSyndromeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSyndromeDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSyndromeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSyndromeDlg)
	DDX_Control(pDX, IDC_VSCROLL2, m_vscroll2);
	DDX_Control(pDX, IDC_BITMAP2, m_frame2);
	DDX_Control(pDX, IDC_BITMAP, m_frame);
	DDX_Control(pDX, IDC_NULLBUTTON, m_nullbtn);
	DDX_Control(pDX, IDC_VSCROLL1, m_vscroll1);
	DDX_Control(pDX, IDC_HSCROLL1, m_hscroll1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSyndromeDlg, CDialog)
	//{{AFX_MSG_MAP(CSyndromeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyndromeDlg message handlers

BOOL CSyndromeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here


	//setup controls
	CString WinTitle;
	WinTitle.Format("Syndrome - %s", APP_VERSION);
	SetWindowText(WinTitle);

	m_nullbtn.ShowWindow(FALSE);
	m_nullbtn.SetFocus();

	SCROLLINFO si;
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nMin = 0;
	si.nMax = CANVAS_H-1;
	si.nPage = SCREEN_H;
	m_vscroll1.SetScrollInfo(&si);
	m_vscroll1.MoveWindow(SCREEN_X+SCREEN_W,SCREEN_Y,SCROLLBAR_W,SCREEN_H);

	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nMin = 0;
	si.nMax = CANVAS_W-1;
	si.nPage = SCREEN_W;
	m_hscroll1.SetScrollInfo(&si);
	m_hscroll1.MoveWindow(SCREEN_X,SCREEN_Y+SCREEN_H,SCREEN_H,SCROLLBAR_W);
	m_frame.MoveWindow(SCREEN_X-2,SCREEN_Y-2,SCREEN_W+SCROLLBAR_W+4,SCREEN_H+SCROLLBAR_W+4);

	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nMin = 0;
	si.nMax = 1600;
	si.nPage = 32;
	m_vscroll2.SetScrollInfo(&si);
	m_vscroll2.MoveWindow(410,SCREEN_Y,SCROLLBAR_W,SCREEN_H+SCROLLBAR_W);
	m_frame2.MoveWindow(284,SCREEN_Y-2,128+SCROLLBAR_W,SCREEN_H+SCROLLBAR_W+4);

	UpdateData(FALSE);

	//open ROM, grab compressed GFX, then decompress
	//fp = fopen("C:\\romhacks\\mm7\\mm7.smc", "rb");
	fp = fopen("C:\\romhacks\\mm7\\decmp\\mm7.smc", "rb");
	if (!fp) MessageBox("Error loading file", "ERROR", MB_ICONERROR);
	fseek(fp,0xF6BA0,SEEK_SET);
	fread(buffer,1,DATA_SIZE,fp);
	decmp_mm7(0x7E00);

	//grab 15-bit palette data and convert to 24-bit
	fseek(fp,0x48640,SEEK_SET);
	fread(buffer,1,0x100,fp);
	fseek(fp,0x48620,SEEK_SET);
	fread(buffer,1,0x20,fp);
	decode_pal();

	//grab TSA data
	fseek(fp,0xB8E00,SEEK_SET);
	fread(TSAData,2,DATA_SIZE,fp);

	//grab structure data
	fseek(fp,0xA5630,SEEK_SET);
	fread(StructData,2,DATA_SIZE,fp);

	//grab map data
	fseek(fp,0xCDB80,SEEK_SET);
	fread(MapData,2,DATA_SIZE,fp);

	//close ROM, clear canvas
	fclose(fp);
	memset(canvas,0,CANVAS_SIZE);

	//draw stuff, copy canvas to bitmap
	int i;
	for (i = 0; i < 18; i++)
		decode_map(i,i,0);
	//CopyCanvas();

	CScreen::Initialize(,SCREEN_X,SCREEN_Y);
	CScreen::Draw(m_frame.GetDC(),SCREEN_X,SCREEN_Y);

	//fp = fopen("C:\\rom hacks\\mm7\\decmp\\mm7-bin.smc", "wb");
	//fwrite(decmp,1,0x7E00,fp);
	//fclose(fp);
	
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CSyndromeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSyndromeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	//RefreshCanvas();
}

void CSyndromeDlg::RefreshCanvas() {
	CDC MemDC,*pDC;
	CBitmap *pOldBitmap;

	pDC = this->GetDC();
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(&m_bitmap);
	
	//pDC->FillSolidRect(SCREEN_X,SCREEN_Y,SCREEN_W,SCREEN_H,RGB(0,0,0));
	pDC->StretchBlt(SCREEN_X,SCREEN_Y,SCREEN_W,SCREEN_H,&MemDC,m_hscroll1.GetScrollPos(),m_vscroll1.GetScrollPos(),SCREEN_W/iZoom,SCREEN_H/iZoom,SRCCOPY);

	//cleanup, stupid windows needs it  >:(
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	ReleaseDC(pDC);
}

void CSyndromeDlg::CopyCanvas() {
	/*
	int x,y,p = 0;

	//copy from canvas to bitmap
	CDC MemDC,*pDC;
	CBitmap *pOldBitmap;

	pDC = this->GetDC();
	MemDC.CreateCompatibleDC(pDC);
	m_bitmap.CreateCompatibleBitmap(pDC,CANVAS_W,CANVAS_H);
	
	pOldBitmap = MemDC.SelectObject(&m_bitmap);

	//MemDC.FillSolidRect(0,0,CANVAS_W,CANVAS_H,RGB(0,0,0)); //erase background
	p=0;
	for (y = 0; y < CANVAS_H; y++) {
		for (x = 0; x < CANVAS_W; x++) {
			MemDC.SetPixel(x,y,canvasPal[canvas[p++]]);
		}
	}
		
	//cleanup, stupid windows needs it  >:(
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	ReleaseDC(pDC);
	*/
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSyndromeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSyndromeDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	m_bitmap.DeleteObject();

	CDialog::OnClose();
}

void CSyndromeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	SCROLLINFO si;
	pScrollBar->GetScrollInfo(&si);

	switch(nSBCode) {
		case SB_BOTTOM:
			break;

		case SB_ENDSCROLL:
			m_nullbtn.SetFocus();
			break;

		case SB_LINEDOWN:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 32);
			break;

		case SB_LINEUP:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 32);
			break;

		case SB_PAGEDOWN:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + si.nPage);
			break;

		case SB_PAGEUP:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - si.nPage);
			break;

		case SB_THUMBPOSITION:
			break;

		case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			break;

		case SB_TOP:
			break;
	}
	
	RefreshCanvas();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSyndromeDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	SCROLLINFO si;
	pScrollBar->GetScrollInfo(&si);

	switch(nSBCode) {
		case SB_BOTTOM:
			break;

		case SB_ENDSCROLL:
			m_nullbtn.SetFocus();
			break;

		case SB_LINEDOWN:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 32);
			break;

		case SB_LINEUP:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 32);
			break;

		case SB_PAGEDOWN:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + si.nPage);
			break;

		case SB_PAGEUP:
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - si.nPage);
			break;

		case SB_THUMBPOSITION:
			break;

		case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			break;

		case SB_TOP:
			break;
	}
	
	RefreshCanvas();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
