#include "PrecompiledHeader.h"
#include "PSSplashScreen.h"

PSSplashScreen* PSSplashScreen::SplashWnd = _In_opt_ NULL;
ATOM PSSplashScreen::szWindowClass = 0;
int PSSplashScreen::m_millisecondsToDisplay = 0;

PSSplashScreen::PSSplashScreen() {}

void PSSplashScreen::ShowSplashScreen(int millisecondsToDisplay /*= 0*/) {

	m_millisecondsToDisplay = millisecondsToDisplay;

	if (SplashWnd == NULL) {
		SplashWnd = new PSSplashScreen();
		if (!SplashWnd->Create()) {
			delete SplashWnd;
			SplashWnd = NULL;
		}
	}

	if (m_millisecondsToDisplay) {
		SetTimer(SplashWnd->m_hwnd, 1, m_millisecondsToDisplay, NULL);
	}

	ShowWindow(SplashWnd->m_hwnd, SW_SHOW);
	UpdateWindow(SplashWnd->m_hwnd);
	InvalidateRect(SplashWnd->m_hwnd, NULL, FALSE);

	SplashWnd->ClearMessageQueue();
}

BOOL PSSplashScreen::RegisterClass(LPCTSTR WindowClassName) {
	m_Instance = GetModuleHandle(NULL);

	WNDCLASSEX wcex{ };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_Instance;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WindowClassName;
	wcex.hCursor = LoadCursor(nullptr, IDC_APPSTARTING);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hIconSm = NULL;

	szWindowClass = RegisterClassEx(&wcex);

	if (szWindowClass == 0)
		return FALSE;

	return TRUE;
}

BOOL PSSplashScreen::Create() {
	m_Instance = GetModuleHandle(NULL);

	m_bitmap = LoadBitmap(m_Instance, MAKEINTRESOURCE(IDB_BITMAP1));

	HWND hwndDesktop = GetDesktopWindow();

	BITMAPINFO bitmapInfo;
	memset(&bitmapInfo, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(GetDC(hwndDesktop),		// handle to DC
		m_bitmap,								// handle to bitmap
		0,										// first scan line to set
		0,										// number of scan lines to copy
		NULL,									// array for bitmap bits
		&bitmapInfo,							// bitmap data buffer
		DIB_RGB_COLORS);						// RGB or palette index

	LPCTSTR szTitle = L"Loading";
	LPCTSTR szWindowClassName = L"SplashScreen";

	if (szWindowClass == 0) {
		BOOL result = RegisterClass(szWindowClassName);
		if (!result)
			return FALSE;
	}

	int xPos = 0;
	int yPos = 0;
	int width = bitmapInfo.bmiHeader.biWidth;
	int height = bitmapInfo.bmiHeader.biHeight;

	RECT parentRect;

	::GetWindowRect(GetDesktopWindow(), &parentRect);

	xPos = parentRect.left + (parentRect.right - parentRect.left) / 2 - (width / 2);
	yPos = parentRect.top + (parentRect.bottom - parentRect.top) / 2 - (height / 2);

	m_hwnd = CreateWindowEx(0, szWindowClassName, szTitle, WS_POPUP,
		xPos, yPos, width, height, NULL, NULL, m_Instance, this);

	if (m_hwnd == NULL) {
		return FALSE;
	}

	::SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);	

	return TRUE;

}

void PSSplashScreen::ClearMessageQueue() {
	MSG msg;
	while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK PSSplashScreen::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message){
	case WM_PAINT:
		SplashWnd->OnPaint(hwnd);
		break;
	case WM_NCDESTROY:
		delete SplashWnd;
		SplashWnd = NULL;
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		DestroyWindow(hwnd);
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void PSSplashScreen::OnPaint(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC paintDC = BeginPaint(hwnd, &ps);

	HDC imageDC = ::CreateCompatibleDC(paintDC);

	BITMAPINFO bitmapInfo;
	memset(&bitmapInfo, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(imageDC,		// handle to DC
		m_bitmap,							// handle to bitmap
		0,									// first scan line to set
		0,									// number of scan lines to copy
		NULL,								// array for bitmap bits
		&bitmapInfo,						// bitmap data buffer
		DIB_RGB_COLORS);					// RGB or palette index

	// Paint the bitmap image.
	HBITMAP pOldBitmap = (HBITMAP)SelectObject(imageDC, m_bitmap);
	int width = bitmapInfo.bmiHeader.biWidth;
	int height = bitmapInfo.bmiHeader.biHeight;
	BitBlt(paintDC, 0, 0, width, height, imageDC, 0, 0, SRCCOPY);
	SelectObject(imageDC, pOldBitmap);

	EndPaint(hwnd, &ps);
}