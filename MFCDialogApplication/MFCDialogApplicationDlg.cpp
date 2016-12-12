
// MFCDialogApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDialogApplication.h"
#include "MFCDialogApplicationDlg.h"
#include "afxdialogex.h"
#include <string>
#include <iostream>
#include "document.h"

#include "HttpRequest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_ID 1

int seconds;
bool isLButtonDown;

int m_iPrevX = 0;
int m_iPrevY = 0;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDialogApplicationDlg dialog



CMFCDialogApplicationDlg::CMFCDialogApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCDIALOGAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMFCDialogApplicationDlg::~CMFCDialogApplicationDlg()
{
	delete pFont;
}

void CMFCDialogApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDialogApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMFCDialogApplicationDlg message handlers

BOOL CMFCDialogApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	m_StaticTimer = (CStatic*)GetDlgItem(IDC_STATIC_TIMER);

	pFont = new CFont;
	pFont->CreatePointFont(500, L"Arial Bold", NULL);

	m_StaticTimer->SetFont(pFont);

	seconds = 5;
	UINT_PTR myTimer = SetTimer(TIMER_ID, 1000, nullptr);

	this->GetWindowRect(&dlgRect);
	m_StaticTimer->GetWindowRect(pos);

	ScreenToClient(&pos);

	const char* json = "{"
		"\"hello\": \"world\","
		"\"t\" : true,"
		"\"f\" : false,"
		"\"n\" : null,"
		"\"i\" : 123,"
		"\"pi\" : 3.1416,"
		"\"a\" : [1, 2, 3, 4]"
		"}";

	rapidjson::Document document;
	document.Parse(json);

	std::string strr;
	assert(document.HasMember("hello"));
	assert(document["hello"].IsString());

	char buff[100];
	snprintf(buff, sizeof(buff), "hello = %s\n", document["hello"].GetString());
	std::string buffAsStdStr = buff;

	std::cout << buffAsStdStr << std::endl;
	OutputDebugStringA(buffAsStdStr.c_str());

	isLButtonDown = false;

	HttpRequestParameters* parameters = new HttpRequestParameters();
	parameters->method = "POST";
	parameters->url = "www.google.com";
	parameters->port = 80;

	parameters->parameters["lang"] = "en";
	parameters->parameters["browser"] = "chrome";

	HttpRequest* request = new HttpRequest(parameters);

	std::string url = request->getUrl();
	std::string method = request->getMethod();

	std::map <std::string, std::string> parameterMap = request->getParameters();
	std::map <std::string, std::string>::iterator iter;

	for(iter = parameterMap.begin(); iter != parameterMap.end(); iter++)
	{
		std::string id = iter->first;
		std::string val = iter->second;
	}

	delete request;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCDialogApplicationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	isLButtonDown = true;

	m_iPrevX = point.x;
	m_iPrevY = point.y;

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCDialogApplicationDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	isLButtonDown = false;

	m_iPrevX = point.x;
	m_iPrevY = point.y;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCDialogApplicationDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (isLButtonDown)
	{
		CRect timerRect;
		m_StaticTimer->GetWindowRect(timerRect);
		ScreenToClient(timerRect);

		if (timerRect.PtInRect(point)) {
			CClientDC dc(this);
			CPen rPen(PS_SOLID, 2, RGB(255, 0, 0));
			dc.SelectObject(&rPen);
			dc.MoveTo(m_iPrevX, m_iPrevY);
			dc.LineTo(point.x, point.y);

			m_iPrevX = point.x;
			m_iPrevY = point.y;
		}	
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CMFCDialogApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCDialogApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

void CMFCDialogApplicationDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_StaticTimer != nullptr)
	{	
		float pw = (float)pos.left / (float)dlgRect.Width();
		float ph = (float)pos.top  / (float)dlgRect.Height();

		int posx = pw * cx;
		int posy = ph * cy;

		m_StaticTimer->MoveWindow(posx, posy, (pos.Width() * cx)/ dlgRect.Width(), (pos.Height() * cy) / dlgRect.Height(), 1);

		//m_StaticTimer->SetWindowPos(NULL, pos.left - 15, pos.top, pos.Width() + 15
		//	, pos.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCDialogApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCDialogApplicationDlg::ExitApp()
{
	// same as double-clicking on main window close box
	ASSERT(AfxGetApp()->m_pMainWnd != NULL);
	AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
}

void CMFCDialogApplicationDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID)
	{
		if (seconds == 0)
		{
			KillTimer(nIDEvent);
			ExitApp();
		}

		std::string str = std::to_string(seconds--);
		std::wstring path_wstr(str.begin(), str.end());
			
		m_StaticTimer->SetWindowTextW(path_wstr.c_str());
	}
}

HBRUSH CMFCDialogApplicationDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		//pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	else
	{
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


