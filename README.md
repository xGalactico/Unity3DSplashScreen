# SplashScreen
 Splash Screen :  Chris Apple 2009, Pedro Silva 2021
 
 # Using the code
* Include SplashScreen.h
* Add the IDB_BITMAP1 bitmap to the WindowsPlayer.rc.
* Include your "resource.h" on PSSplashScreen.cpp

```cpp
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    PSSplashScreen::ShowSplashScreen(5500);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return UnityMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
```
