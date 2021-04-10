#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>

#include "wil/com.h"
#include "wil/resource.h"

#include <thread>

// A simple manager for the window handle and the D3D11 device
class RenderWindow
{
public:
	RenderWindow(HINSTANCE instance, int cmdShow);
	~RenderWindow();

	ID3D11Device* GetD3D11Device() const { return m_device.get(); }

private:
	std::thread m_windowThread;
	wil::unique_hwnd m_window;

	wil::com_ptr<ID3D11Device> m_device;
	wil::com_ptr<ID3D11DeviceContext> m_deviceContext;
	wil::com_ptr<IDXGISwapChain> m_swapChain;
};