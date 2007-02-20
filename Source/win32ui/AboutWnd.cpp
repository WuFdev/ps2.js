#include <zlib.h>
#include "AboutWnd.h"
#include "PtrMacro.h"
#include "LayoutStretch.h"
#include "HorizontalLayout.h"
#include "win32/LayoutWindow.h"
#include "resource.h"
#include "../PS2VM.h"
#include "string_cast.h"

#define CLSNAME		_T("AboutWnd")
#define WNDSTYLE	(WS_CAPTION | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU)
#define WNDSTYLEEX	(WS_EX_DLGMODALFRAME)

using namespace Framework;
using namespace std;

CAboutWnd::CAboutWnd(HWND hParent)
{
	RECT rc;

	TCHAR sVersion[256];

	CVerticalLayout* pSubLayout0;
	CVerticalLayout* pSubLayout1;
	CHorizontalLayout* pSubLayout2;

	if(!DoesWindowClassExist(CLSNAME))
	{
		WNDCLASSEX w;
		memset(&w, 0, sizeof(WNDCLASSEX));
		w.cbSize		= sizeof(WNDCLASSEX);
		w.lpfnWndProc	= CWindow::WndProc;
		w.lpszClassName	= CLSNAME;
		w.hbrBackground	= (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
		w.hInstance		= GetModuleHandle(NULL);
		w.hCursor		= LoadCursor(NULL, IDC_ARROW);
		RegisterClassEx(&w);
	}

	SetRect(&rc, 0, 0, 300, 101);

	if(hParent != NULL)
	{
		EnableWindow(hParent, FALSE);
	}

	Create(WNDSTYLEEX, CLSNAME, _T("About プレイ!"), WNDSTYLE, &rc, hParent, NULL);
	SetClassPtr();

	SetRect(&rc, 0, 0, 1, 1);

	m_pImage = new Win32::CStatic(m_hWnd, &rc, SS_ICON);
	m_pImage->SetIcon(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PUREI), IMAGE_ICON, 48, 48, 0));

    tstring sDate(string_cast<tstring>(__DATE__));
    tstring sZlibVersion(string_cast<tstring>(ZLIB_VERSION));

	_sntprintf(sVersion, countof(sVersion), _T("Version %i.%0.2i (%s) - zlib v%s"), VERSION_MAJOR, VERSION_MINOR, sDate.c_str(), sZlibVersion.c_str());

	pSubLayout0 = new CVerticalLayout(3);
	pSubLayout0->InsertObject(CLayoutWindow::CreateTextBoxBehavior(100, 15, new Win32::CStatic(m_hWnd, sVersion)));
	pSubLayout0->InsertObject(CLayoutWindow::CreateTextBoxBehavior(100, 15, new Win32::CStatic(m_hWnd, _T("Written by Jean-Philip Desjardins"))));
	pSubLayout0->InsertObject(CLayoutWindow::CreateTextBoxBehavior(100, 15, new Win32::CStatic(m_hWnd, _T("jean-philip.desjardins@polymtl.ca"))));

	pSubLayout0->InsertObject(new CLayoutStretch);
	pSubLayout0->SetHorizontalStretch(1);

	pSubLayout1 = new CVerticalLayout;
	pSubLayout1->InsertObject(new CLayoutWindow(48, 48, 0, 0, m_pImage));
	pSubLayout1->InsertObject(new CLayoutStretch);

	pSubLayout2 = new CHorizontalLayout(10);
	pSubLayout2->InsertObject(pSubLayout1);
	pSubLayout2->InsertObject(pSubLayout0);
	pSubLayout2->SetVerticalStretch(0);

	m_pLayout = new CVerticalLayout;
	m_pLayout->InsertObject(pSubLayout2);
	m_pLayout->InsertObject(new CLayoutStretch);

	RefreshLayout();
}

CAboutWnd::~CAboutWnd()
{
	DELETEPTR(m_pLayout);
}

long CAboutWnd::OnSysCommand(unsigned int nCmd, LPARAM lParam)
{
	switch(nCmd)
	{
	case SC_CLOSE:
		if(GetParent() != NULL)
		{
			EnableWindow(GetParent(), TRUE);
			SetForegroundWindow(GetParent());
		}
		break;
	}
	return TRUE;
}

void CAboutWnd::RefreshLayout()
{
	RECT rc;

	GetClientRect(&rc);

	SetRect(&rc, rc.left + 10, rc.top + 10, rc.right - 10, rc.bottom - 10);

	m_pLayout->SetRect(rc.left, rc.top, rc.right, rc.bottom);
	m_pLayout->RefreshGeometry();

	Redraw();
}
