// SyndromeDlg.h : header file
//

#if !defined(AFX_SYNDROMEDLG_H__DAF2F5BA_D0FC_11D6_B5C0_00C0F05B64CB__INCLUDED_)
#define AFX_SYNDROMEDLG_H__DAF2F5BA_D0FC_11D6_B5C0_00C0F05B64CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// CScreen bitmap

class CScreen {
public:
	CScreen(); //constructor
	~CScreen(); //destructor

	void Initialize(HDC hScreenDC, int w, int h);
	void Draw(HDC hDestDC, int w, int h);
	void Destroy();

protected:
	HDC hDC;
	HBITMAP hBmp;
	HBITMAP hBmpOld;
};


CScreen::CScreen() {
	hDC = NULL;
	hBmp = NULL;
	hBmpOld = NULL;
}

CScreen::~CScreen() {
	Destroy();
}

void CScreen::Initialize(HDC hScreenDC, int w, int h) {
	if (hDC) return; //don't initialize if it's already initialized

	hDC = CreateCompatibleDC(hScreenDC);
	hBmp = CreateCompatibleBitmap(hScreenDC,w,h);
	hBmpOld = (HBITMAP)SelectObject(hDC,hBmp);
}

void CScreen::Draw(HDC hDestDC, int w, int h) {
	StretchBlt(hDestDC,0,0,w,h, hDC,0,0,w,h, SRCCOPY);
	//(SCREEN_X,SCREEN_Y,SCREEN_W,SCREEN_H,&MemDC,m_hscroll1.GetScrollPos(),m_vscroll1.GetScrollPos(),SCREEN_W/iZoom,SCREEN_H/iZoom,SRCCOPY);
}

void CScreen::Destroy() {
	if (!hDC) return; //don't destroy if there's nothing do destroy

	SelectObject(hDC,hBmpOld);
	DeleteDC(hDC);
	DeleteObject(hBmp);

	hDC = NULL;
	hBmp = NULL;
	hBmpOld = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CSyndromeDlg dialog

class CSyndromeDlg : public CDialog
{
public:
	void RefreshCanvas();
	void CopyCanvas();
	CBitmap m_bitmap;
	CBitmap m_tile;

// Construction
public:
	CSyndromeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSyndromeDlg)
	enum { IDD = IDD_SYNDROME_DIALOG };
	CScrollBar	m_vscroll2;
	CStatic	m_frame2;
	CStatic	m_frame;
	CButton	m_nullbtn;
	CScrollBar	m_vscroll1;
	CScrollBar	m_hscroll1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSyndromeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSyndromeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNDROMEDLG_H__DAF2F5BA_D0FC_11D6_B5C0_00C0F05B64CB__INCLUDED_)
