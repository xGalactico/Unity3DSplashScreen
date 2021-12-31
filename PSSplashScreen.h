#pragma once

class PSSplashScreen {
public:
	PSSplashScreen();
	static void ShowSplashScreen(int millisecondsToDisplay /*= 0*/);
protected:
	BOOL Create();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL RegisterClass(LPCTSTR szWindowClassName);
	void ClearMessageQueue();
	void OnPaint(HWND hwnd);
private:
	HICON m_hicon;
	HWND m_hwnd;
	HINSTANCE m_Instance;
	static PSSplashScreen* SplashWnd;
	static ATOM szWindowClass;
	HBITMAP m_bitmap;
	static int m_millisecondsToDisplay;
};