#include <Windows.h>
#include <D3D11.h>
#include <D3DX11.h>

// specifies if the game will run on a single thread
// #define SINGLE_THREADED 0
// defines the window title
#define WINDOW_TITLE L"Discovery"
// defines the window width
#define WINDOW_WIDTH 1280
// defines thewindow height
#define WINDOW_HEIGHT 720
// defines the fixed border window style
#define WS_FIXEDSIZE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)