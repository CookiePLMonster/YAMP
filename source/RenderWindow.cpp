#include "RenderWindow.h"

#include "wil/win32_helpers.h"

#include <DirectXMath.h>

#include <algorithm>

#pragma comment(lib, "d3d11.lib")

static constexpr DXGI_FORMAT OUTPUT_FORMAT = DXGI_FORMAT_B8G8R8A8_UNORM;

static LRESULT WINAPI WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

RenderWindow::RenderWindow(HINSTANCE instance, HINSTANCE dllInstance, int cmdShow)
{
	wil::unique_event startupEvent(wil::EventOptions::None);
	m_windowThread = std::thread([this, &startupEvent, instance, dllInstance, cmdShow] {
		
		WNDCLASSEX wndClass { sizeof(wndClass) };
		wndClass.hInstance = instance;
		wndClass.lpfnWndProc = WindowProc;
		wndClass.lpszClassName = L"YAKUZA_VF5FS";
		wndClass.hIcon = LoadIcon(dllInstance, MAKEINTRESOURCE(101));
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

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
		
#ifdef _DEBUG
		UINT Flags = 0;//D3D11_CREATE_DEVICE_DEBUG;
#else
		UINT Flags = 0;
#endif

		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, Flags, nullptr, 0, D3D11_SDK_VERSION, device.addressof(), nullptr, deviceContext.addressof());
		THROW_IF_FAILED(hr);

		wil::com_ptr<IDXGISwapChain> swapChain = CreateSwapChainForWindow(device.get(), window.get());

		ShowWindow(window.get(), cmdShow);
		UpdateWindow(window.get());

		m_window = std::move(window);
		m_device = std::move(device);
		m_deviceContext = std::move(deviceContext);
		m_swapChain = std::move(swapChain);

		CreateRenderResources();
		EnumerateDisplayModes();
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

		m_shuttingDownWindow.store(true, std::memory_order_relaxed);

		// TODO: Pass the exit code back to WinMain via RenderWindow
		
	});
	startupEvent.wait();
}

RenderWindow::~RenderWindow()
{
	PostMessage(m_window.get(), WM_CLOSE, 0, 0);
	m_windowThread.join();
}

void RenderWindow::BlitGameFrame(ID3D11ShaderResourceView* src)
{
	m_deviceContext->OMSetRenderTargets(1, m_backBufferRTV.addressof(), nullptr);
	m_deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

	m_deviceContext->VSSetShader(m_vs.get(), nullptr, 0);

	const UINT Offsets[1] = { 0 };
	m_deviceContext->IASetInputLayout(m_inputLayout.get());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_deviceContext->IASetVertexBuffers(0, 1, m_vb.addressof(), &m_vbStride, Offsets);

	m_deviceContext->PSSetShader(m_ps.get(), nullptr, 0);
	m_deviceContext->PSSetShaderResources(0, 1, &src);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = GetWidth();
	viewport.Height = GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_deviceContext->RSSetViewports(1, &viewport);

	m_deviceContext->Draw(3, 0);
}

wil::com_ptr<IDXGISwapChain> RenderWindow::CreateSwapChainForWindow(ID3D11Device* device, HWND window)
{
	wil::com_ptr<IDXGISwapChain> swapChain;

	DXGI_SWAP_CHAIN_DESC swapChainDesc {};
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = OUTPUT_FORMAT;
	swapChainDesc.SampleDesc.Count = 1;                             
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = TRUE;

	wil::com_ptr<IDXGIDevice> dxgiDevice;
	HRESULT hr = device->QueryInterface(IID_PPV_ARGS(dxgiDevice.addressof()));
	THROW_IF_FAILED(hr);

	wil::com_ptr<IDXGIAdapter> adapter;
	hr = dxgiDevice->GetAdapter(adapter.addressof());
	THROW_IF_FAILED(hr);

	wil::com_ptr<IDXGIFactory> factory;
	hr = adapter->GetParent(IID_PPV_ARGS(factory.addressof()));
	THROW_IF_FAILED(hr);

	hr = factory->CreateSwapChain(device, &swapChainDesc, swapChain.addressof());
	THROW_IF_FAILED(hr);

	return swapChain;
}

void RenderWindow::CreateRenderResources()
{
	ID3D11Device* device = m_device.get();

	// Create the back buffer RTV
	wil::com_ptr<ID3D11Resource> backBuffer;
	HRESULT hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.addressof()));
	THROW_IF_FAILED(hr);
	hr = device->CreateRenderTargetView(backBuffer.get(), nullptr, m_backBufferRTV.addressof());
	THROW_IF_FAILED(hr);

	// Create shaders
	static const uint8_t vs_main[] =
	{
		 68,  88,  66,  67,   0, 158, 
		 39, 114, 142,  75, 234,  29, 
		 89, 242, 219,   6,  14, 240, 
		 60,  96,   1,   0,   0,   0, 
		 56,   2,   0,   0,   5,   0, 
		  0,   0,  52,   0,   0,   0, 
		128,   0,   0,   0, 212,   0, 
		  0,   0,  44,   1,   0,   0, 
		188,   1,   0,   0,  82,  68, 
		 69,  70,  68,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		 28,   0,   0,   0,   0,   4, 
		254, 255,   0, 129,   0,   0, 
		 28,   0,   0,   0,  77, 105, 
		 99, 114, 111, 115, 111, 102, 
		116,  32,  40,  82,  41,  32, 
		 72,  76,  83,  76,  32,  83, 
		104,  97, 100, 101, 114,  32, 
		 67, 111, 109, 112, 105, 108, 
		101, 114,  32,  49,  48,  46, 
		 49,   0,  73,  83,  71,  78, 
		 76,   0,   0,   0,   2,   0, 
		  0,   0,   8,   0,   0,   0, 
		 56,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  3,   0,   0,   0,   0,   0, 
		  0,   0,   3,   3,   0,   0, 
		 65,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  3,   0,   0,   0,   1,   0, 
		  0,   0,   3,   3,   0,   0, 
		 80,  79,  83,  73,  84,  73, 
		 79,  78,   0,  84,  69,  88, 
		 67,  79,  79,  82,  68,   0, 
		171, 171,  79,  83,  71,  78, 
		 80,   0,   0,   0,   2,   0, 
		  0,   0,   8,   0,   0,   0, 
		 56,   0,   0,   0,   0,   0, 
		  0,   0,   1,   0,   0,   0, 
		  3,   0,   0,   0,   0,   0, 
		  0,   0,  15,   0,   0,   0, 
		 68,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  3,   0,   0,   0,   1,   0, 
		  0,   0,   3,  12,   0,   0, 
		 83,  86,  95,  80,  79,  83, 
		 73,  84,  73,  79,  78,   0, 
		 84,  69,  88,  67,  79,  79, 
		 82,  68,   0, 171, 171, 171, 
		 83,  72,  68,  82, 136,   0, 
		  0,   0,  64,   0,   1,   0, 
		 34,   0,   0,   0,  95,   0, 
		  0,   3,  50,  16,  16,   0, 
		  0,   0,   0,   0,  95,   0, 
		  0,   3,  50,  16,  16,   0, 
		  1,   0,   0,   0, 103,   0, 
		  0,   4, 242,  32,  16,   0, 
		  0,   0,   0,   0,   1,   0, 
		  0,   0, 101,   0,   0,   3, 
		 50,  32,  16,   0,   1,   0, 
		  0,   0,  54,   0,   0,   5, 
		 50,  32,  16,   0,   0,   0, 
		  0,   0,  70,  16,  16,   0, 
		  0,   0,   0,   0,  54,   0, 
		  0,   8, 194,  32,  16,   0, 
		  0,   0,   0,   0,   2,  64, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		128,  63,   0,   0, 128,  63, 
		 54,   0,   0,   5,  50,  32, 
		 16,   0,   1,   0,   0,   0, 
		 70,  16,  16,   0,   1,   0, 
		  0,   0,  62,   0,   0,   1, 
		 83,  84,  65,  84, 116,   0, 
		  0,   0,   4,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   4,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  1,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  3,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0
	};
	hr = device->CreateVertexShader(vs_main, sizeof(vs_main), nullptr, m_vs.addressof());
	THROW_IF_FAILED(hr);

	const D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device->CreateInputLayout(inputElements, std::size(inputElements), vs_main, sizeof(vs_main), m_inputLayout.addressof());
	THROW_IF_FAILED(hr);

	static const uint8_t ps_main[] =
	{
		 68,  88,  66,  67, 106, 118, 
		 18, 107, 193, 156, 216,  42, 
		  8, 105, 148, 229, 179, 147, 
		188, 143,   1,   0,   0,   0, 
		 88,   2,   0,   0,   5,   0, 
		  0,   0,  52,   0,   0,   0, 
		228,   0,   0,   0,  60,   1, 
		  0,   0, 112,   1,   0,   0, 
		220,   1,   0,   0,  82,  68, 
		 69,  70, 168,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   2,   0,   0,   0, 
		 28,   0,   0,   0,   0,   4, 
		255, 255,   0, 129,   0,   0, 
		128,   0,   0,   0,  92,   0, 
		  0,   0,   3,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   1,   0, 
		  0,   0,   1,   0,   0,   0, 
		110,   0,   0,   0,   2,   0, 
		  0,   0,   5,   0,   0,   0, 
		  4,   0,   0,   0, 255, 255, 
		255, 255,   0,   0,   0,   0, 
		  1,   0,   0,   0,  13,   0, 
		  0,   0,  98,  97,  99, 107, 
		 66, 117, 102, 102, 101, 114, 
		 83,  97, 109, 112, 108, 101, 
		114,   0,  98,  97,  99, 107, 
		 66, 117, 102, 102, 101, 114, 
		 84, 101, 120, 116, 117, 114, 
		101,   0,  77, 105,  99, 114, 
		111, 115, 111, 102, 116,  32, 
		 40,  82,  41,  32,  72,  76, 
		 83,  76,  32,  83, 104,  97, 
		100, 101, 114,  32,  67, 111, 
		109, 112, 105, 108, 101, 114, 
		 32,  49,  48,  46,  49,   0, 
		 73,  83,  71,  78,  80,   0, 
		  0,   0,   2,   0,   0,   0, 
		  8,   0,   0,   0,  56,   0, 
		  0,   0,   0,   0,   0,   0, 
		  1,   0,   0,   0,   3,   0, 
		  0,   0,   0,   0,   0,   0, 
		 15,   0,   0,   0,  68,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   3,   0, 
		  0,   0,   1,   0,   0,   0, 
		  3,   3,   0,   0,  83,  86, 
		 95,  80,  79,  83,  73,  84, 
		 73,  79,  78,   0,  84,  69, 
		 88,  67,  79,  79,  82,  68, 
		  0, 171, 171, 171,  79,  83, 
		 71,  78,  44,   0,   0,   0, 
		  1,   0,   0,   0,   8,   0, 
		  0,   0,  32,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   3,   0,   0,   0, 
		  0,   0,   0,   0,  15,   0, 
		  0,   0,  83,  86,  95,  84, 
		 65,  82,  71,  69,  84,   0, 
		171, 171,  83,  72,  68,  82, 
		100,   0,   0,   0,  64,   0, 
		  0,   0,  25,   0,   0,   0, 
		 90,   0,   0,   3,   0,  96, 
		 16,   0,   0,   0,   0,   0, 
		 88,  24,   0,   4,   0, 112, 
		 16,   0,   0,   0,   0,   0, 
		 85,  85,   0,   0,  98,  16, 
		  0,   3,  50,  16,  16,   0, 
		  1,   0,   0,   0, 101,   0, 
		  0,   3, 242,  32,  16,   0, 
		  0,   0,   0,   0,  69,   0, 
		  0,   9, 242,  32,  16,   0, 
		  0,   0,   0,   0,  70,  16, 
		 16,   0,   1,   0,   0,   0, 
		 70, 126,  16,   0,   0,   0, 
		  0,   0,   0,  96,  16,   0, 
		  0,   0,   0,   0,  62,   0, 
		  0,   1,  83,  84,  65,  84, 
		116,   0,   0,   0,   2,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   2,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   1,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  1,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0
	};
	hr = device->CreatePixelShader(ps_main, sizeof(ps_main), nullptr, m_ps.addressof());
	THROW_IF_FAILED(hr);

	// Create a vertex buffer
	{
		using namespace DirectX;

		static const struct
		{
			XMFLOAT2 Position;
			XMFLOAT2 Texcoord0;
		} vertexBuffer[] = {
			{ XMFLOAT2(-1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT2(-1.0f, -3.0f), XMFLOAT2(0.0f, 2.0f) },
			{ XMFLOAT2(3.0f, 1.0f), XMFLOAT2(2.0f, 0.0f) },
		};
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(vertexBuffer);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		const D3D11_SUBRESOURCE_DATA initData { vertexBuffer };
		hr = device->CreateBuffer(&desc, &initData, m_vb.addressof());
		THROW_IF_FAILED(hr);

		m_vbStride = sizeof(vertexBuffer[0]);
	}
}

void RenderWindow::EnumerateDisplayModes()
{
	auto dxgiDevice = m_device.query<IDXGIDevice>();
	wil::com_ptr<IDXGIAdapter> adapter;
	HRESULT hr = dxgiDevice->GetAdapter(adapter.addressof());
	THROW_IF_FAILED(hr);

	wil::com_ptr<IDXGIOutput> output;
	hr = adapter->EnumOutputs(0, output.addressof());
	THROW_IF_FAILED(hr);

	UINT numModes = 0;
	hr = output->GetDisplayModeList(OUTPUT_FORMAT, 0, &numModes, nullptr);
	THROW_IF_FAILED(hr);

	auto displayModes = std::make_unique<DXGI_MODE_DESC[]>(numModes);
	output->GetDisplayModeList(OUTPUT_FORMAT, 0, &numModes, displayModes.get());
	for (const auto& mode : wil::make_range(displayModes.get(), numModes))
	{
		if (mode.ScanlineOrdering != DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE || mode.Scaling != DXGI_MODE_SCALING_UNSPECIFIED) continue;

		m_displayModes.push_back({mode.Width, mode.Height, static_cast<float>(mode.RefreshRate.Numerator) / mode.RefreshRate.Denominator});
	}

	// Refresh rates seem to be unsorted, fix it
	std::sort(m_displayModes.begin(), m_displayModes.end(), [](const DisplayMode& left, const DisplayMode& right) {
		return std::tie(left.Width, left.Height, left.RefreshRate) < std::tie(right.Width, right.Height, right.RefreshRate);
	});
}
