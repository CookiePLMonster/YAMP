#include "gs.h"

#include "../RenderWindow.h"

namespace gs {

context_t* sm_context;

}

namespace sbgl {

void cgs_device_context::initialize(ccontext* p_context)
{
	mp_sbgl_context = p_context;
}

void cdevice::initialize(const RenderWindow& renderWindow)
{
	m_swap_chain.initialize(renderWindow);
	m_pD3DDeviceContext = renderWindow.GetD3D11DeviceContext();
}

HRESULT cswap_chain_common::initialize(const RenderWindow& window)
{
	// TODO: Validate
	constexpr sbgl_format_t depth_format = SBGL_FORMAT_D32_F_S8X24_U;

	ID3D11Device* device = window.GetD3D11Device();

	m_pDXGISwapChain = window.GetSwapChain();

	wil::com_ptr<ID3D11Texture2D> swapChainBuffer;
	HRESULT hr = m_pDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(swapChainBuffer.addressof()));
	if (SUCCEEDED(hr))
	{
		hr = device->CreateRenderTargetView(swapChainBuffer.get(), nullptr, &m_pD3DRenderTargetView);
		if (SUCCEEDED(hr))
		{
			D3D11_TEXTURE2D_DESC dsDesc;
			dsDesc.Width = window.GetWidth();
			dsDesc.Height = window.GetHeight();
			dsDesc.MipLevels = 1;
			dsDesc.ArraySize = 1;
			dsDesc.Format = static_cast<DXGI_FORMAT>((depth_format >> 7) & 0x7F);
			dsDesc.SampleDesc.Count = 1;
			dsDesc.SampleDesc.Quality = 0;
			dsDesc.Usage = D3D11_USAGE_DEFAULT;
			dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			dsDesc.CPUAccessFlags = 0;
			dsDesc.MiscFlags = 0;
			hr = device->CreateTexture2D(&dsDesc, nullptr, &m_pD3DDepthStencilBuffer);
			if (SUCCEEDED(hr))
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
				dsvDesc.Format = static_cast<DXGI_FORMAT>(depth_format & 0x7F);
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Flags = 0;
				dsvDesc.Texture2D.MipSlice = 0;
				hr = device->CreateDepthStencilView(m_pD3DDepthStencilBuffer, &dsvDesc, &m_pD3DDepthStencilView);
			}
		}
	}
	return hr;
}

}