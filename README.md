# SplashScreen
 Splash Screen :  Chris Apple 2009, Pedro Silva 2021
 
 # Using the code
* In the build settings check Create Visual Studio Solution.
* Include your "resource.h" in PSSplashScreen.cpp.
* Add a bitmap image to WindowsPlayer.rc.
* Include SplashScreen.h in Main.cpp.

```cpp
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    PSSplashScreen::ShowSplashScreen(5500); //millisecondsToDisplay

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return UnityMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
```

# Image
```cpp
BOOL PSSplashScreen::Create() {
    m_bitmap = LoadBitmap(m_Instance, MAKEINTRESOURCE(IDB_BITMAP1)); // IDB_BITMAP1 image.bmp in WindowsPlayer.rc
    
    LPCTSTR szTitle = L"Loading"; // Window Title
    LPCTSTR szWindowClassName = L"SplashScreen"; // Window Class
}
```
