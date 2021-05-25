#pragma once

#include "YAMPUserInterface.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>

#include "wil/com.h"
#include "wil/resource.h"

#include <atomic>
#include <thread>
#include <vector>

// A simple manager for the window handle and the D3D11 device
class RenderWindow
{
public:
	RenderWindow(HINSTANCE instance, HINSTANCE dllInstance, int cmdShow);
	~RenderWindow();

	ID3D11Device* GetD3D11Device() const { return m_device.get(); }
	ID3D11DeviceContext* GetD3D11DeviceContext() const { return m_deviceContext.get(); }
	IDXGISwapChain* GetSwapChain() const { return m_swapChain.get(); }

	void BlitGameFrame(ID3D11ShaderResourceView* src);

	void NewImGuiFrame();
	void RenderImGui();

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

	bool IsShuttingDown() const { return m_shuttingDownWindow.load(std::memory_order_relaxed); }

private:
	wil::com_ptr<IDXGISwapChain> CreateSwapChainForWindow(ID3D11Device* device, HWND window);
	void CreateRenderResources();
	void EnumerateDisplayModes();
	void CalculateViewport();

	std::atomic_bool m_shuttingDownWindow { false };
	std::thread m_windowThread;
	wil::unique_hwnd m_window;

	wil::com_ptr<ID3D11Device> m_device;
	wil::com_ptr<ID3D11DeviceContext> m_deviceContext;
	wil::com_ptr<IDXGISwapChain> m_swapChain;

	// Render resources required to render to backbuffer
	wil::com_ptr<ID3D11RenderTargetView> m_backBufferRTV;
	wil::com_ptr<ID3D11VertexShader> m_vs;
	wil::com_ptr<ID3D11PixelShader> m_ps;
	wil::com_ptr<ID3D11InputLayout> m_inputLayout;
	wil::com_ptr<ID3D11Buffer> m_vb;
	UINT m_vbStride;
	D3D11_VIEWPORT m_viewport;
	bool m_requiresClear = false;

	uint32_t m_width, m_height;

	YAMPUserInterface m_ui;
};