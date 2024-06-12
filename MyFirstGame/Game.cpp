#include "Game.h"

// stores the device
ID3D11Device* g_pd3dDevice;
// stores the device context
ID3D11DeviceContext* g_pImmediateContext;
// stores the swap chain
IDXGISwapChain* g_pSwapChain;
// stores the render targte view
ID3D11RenderTargetView* g_pRenderTargetView;

// handles any message sent to the window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// checks the recieved message
	switch (uMsg)
	{
	case WM_DESTROY:
		// posts a quite message to the window
		PostQuitMessage(0);
		break;
	default:
		// lets windows handle the message
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	// tells windows we have delt with the message
	return 0;
}

// initializes directx
BOOL InitDevice(HWND hWindow, DWORD dwWidth, DWORD dwHeight)
{
	// stores the back buffer
	ID3D11Texture2D* pBackBuffer;
	// creates the swap chain description
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	// zeroes out the structure
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	// sets the buffer count to one buffer
	SwapChainDesc.BufferCount = 1;
	// sets the width of the buffer
	SwapChainDesc.BufferDesc.Width = dwWidth;
	// sets the height of the buffer
	SwapChainDesc.BufferDesc.Height = dwHeight;
	// sets the buffer pixel format
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// sets the refresh rate numerator
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	// sets the refresh rate denominator
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// sets the usage of the buffer
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// sets the output window of the buffer
	SwapChainDesc.OutputWindow = hWindow;
	// sets teh buffer count
	SwapChainDesc.SampleDesc.Count = 1;
	// sets the buffer quality
	SwapChainDesc.SampleDesc.Quality = 0;
	// sets the buffer to windowed
	SwapChainDesc.Windowed = TRUE;
	// creates the device and swapchain
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pImmediateContext))) return FALSE;
	// creates a render target view
	if (FAILED(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) return FALSE;
	// creates a render target view
	HRESULT Result = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	// releases the back buffer
	pBackBuffer->Release();
	// validates the status
	if (FAILED(Result)) return FALSE;
	// sets the render targets
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	// creates a view port
	D3D11_VIEWPORT ViewPort;
	// sets the view port width
	ViewPort.Width = (FLOAT)dwWidth;
	// sets the view port height
	ViewPort.Height = (FLOAT)dwHeight;
	// sets the minimum depth of the view port
	ViewPort.MinDepth = 0.0f;
	// sets the maxium depth of the view port
	ViewPort.MaxDepth = 1.0f;
	// set the x posiiton
	ViewPort.TopLeftX = 0;
	// sets the y position
	ViewPort.TopLeftY = 0;
	// applies the view port of the context
	g_pImmediateContext->RSSetViewports(1, &ViewPort);
	// function succeeded
	return TRUE;
}

// initializes the game window
HWND InitWindow(HINSTANCE hInstance, DWORD dwWidth, DWORD dwHeight, int nCmdShow)
{
	// creates a window class
	WNDCLASSEX WindowClass;
	// zeros out the structure
	ZeroMemory(&WindowClass, sizeof(WNDCLASSEX));
	// sets the window class size
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	// sets the style of the window
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// sets the window message handler
	WindowClass.lpfnWndProc = WindowProc;
	// sets the module instance of the window
	WindowClass.hInstance = hInstance;
	// sets the window cursor
	WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	// sets the window background colour
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	// sets the name used to referance this class
	WindowClass.lpszClassName = L"GameWindow";
	// registers the window class
	if (!RegisterClassEx(&WindowClass)) return nullptr;
	// creates the window resolution
	RECT Resolution = { 0, 0, dwWidth, dwHeight };
	// adjusts the window resolution for the style
	if (!AdjustWindowRect(&Resolution, WS_FIXEDSIZE, FALSE)) return nullptr;
	// creates the window
	HWND hWindow = CreateWindowW(L"GameWindow", WINDOW_TITLE, WS_FIXEDSIZE, CW_USEDEFAULT, CW_USEDEFAULT, Resolution.right - Resolution.left, Resolution.bottom - Resolution.top, nullptr, nullptr, hInstance, nullptr);
	// validates the window
	if (!hWindow) return nullptr;
	// shows the window
	ShowWindow(hWindow, nCmdShow);
	// window was created successfully
	return hWindow;
}

// a render procedure that is called every frame
void Render()
{
	// the color to clear the back buffer to
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// clears the back buffer
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	// swaps the front buffer with the back buffer
	g_pSwapChain->Present(0, 0);
}

// the main entry point of the game
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// initalizes the game window
	if (HWND hWindow = InitWindow(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, nCmdShow))
	{
		// initializes the device on the game window
		if (InitDevice(hWindow, WINDOW_WIDTH, WINDOW_HEIGHT))
		{
			// stores the window message
			MSG RecievedMessage;
			// enters the window message loop
			while (RecievedMessage.message != WM_QUIT)
			{
				// deals with any pending messages
				if (PeekMessageW(&RecievedMessage, nullptr, 0, 0, PM_REMOVE))
				{
					// translates the message
					TranslateMessage(&RecievedMessage);
					// sends the message to the appropriate handler
					DispatchMessage(&RecievedMessage);
				}
				else
				{
					// renders a frame
					Render();
				}
			}
			// returns the quit message parameter
			return (int)RecievedMessage.wParam;
		}
	}
	// start up failed
	return 0;
}