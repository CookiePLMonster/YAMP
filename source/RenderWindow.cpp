#include "RenderWindow.h"

#include "wil/win32_helpers.h"

#pragma comment(lib, "d3d11.lib")

RenderWindow::RenderWindow(HINSTANCE instance, HINSTANCE dllInstance, int cmdShow)
{
	wil::unique_event startupEvent(wil::EventOptions::None);
	m_windowThread = std::thread([this, &startupEvent, instance, dllInstance, cmdShow] {
		
		WNDCLASSEX wndClass { sizeof(wndClass) };
		wndClass.hInstance = instance;
		wndClass.lpfnWndProc = DefWindowProc;
		wndClass.lpszClassName = L"YAKUZA_VF5FS";
		wndClass.hIcon = LoadIcon(dllInstance, MAKEINTRESOURCE(101));

		const ATOM windowClass = RegisterClassEx(&wndClass);
		THROW_LAST_ERROR_IF(windowClass == 0);

		RECT clientArea { 0, 0, 1280, 720 };
		AdjustWindowRect(&clientArea, WS_OVERLAPPEDWINDOW, FALSE);

		wil::unique_hwnd window(CreateWindowExW(0, L"YAKUZA_VF5FS", L"Virtua Fighter 5: Final Showdown", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			clientArea.right - clientArea.left, clientArea.bottom - clientArea.top, nullptr, nullptr, instance, nullptr));
		THROW_LAST_ERROR_IF_NULL(window);

		// TODO: Set up proper feature levels and a debug layer
		wil::com_ptr<ID3D11Device> device;
		wil::com_ptr<ID3D11DeviceContext> deviceContext;
		wil::com_ptr<IDXGISwapChain> swapChain;

		DXGI_SWAP_CHAIN_DESC swapChainDesc {};
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;                             
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = window.get();
		swapChainDesc.Windowed = TRUE;

#ifdef _DEBUG
		UINT Flags = D3D11_CREATE_DEVICE_DEBUG;
#else
		UINT Flags = 0;
#endif

		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, Flags, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
			swapChain.addressof(), device.addressof(), nullptr, deviceContext.addressof());
		THROW_IF_FAILED(hr);

		ShowWindow(window.get(), cmdShow);
		UpdateWindow(window.get());

		m_window = std::move(window);
		m_device = std::move(device);
		m_deviceContext = std::move(deviceContext);
		m_swapChain = std::move(swapChain);
		startupEvent.SetEvent();

		BOOL ret;
		MSG msg;
		while( (ret = GetMessage(&msg, NULL, 0, 0)) != 0)
		{ 
			if (ret == -1)
			{
				// handle the error and possibly exit
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		} 


		// TODO: Pass the exit code back to WinMain via RenderWindow
		
	});
	startupEvent.wait();
}

RenderWindow::~RenderWindow()
{
}