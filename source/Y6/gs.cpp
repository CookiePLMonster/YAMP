#include "gs.h"

#include "../RenderWindow.h"

namespace gs {

context_t* sm_context;

}

void cgs_device_context::initialize(sbgl::ccontext* p_context)
{
	if (p_context != nullptr)
	{
		mp_sbgl_context = p_context;

		// Deliberately left as nullptr now, potentially unused (the DLL allocates its own pools)
		mp_cb_pool = nullptr;
		mp_up_pool = nullptr;
		mp_shader_uniform = gs::sm_context->stack_shader_uniform.pop();
		reset_state_all();

		// TODO: Yakuza 6 returns an error code here??
	}
}

namespace sbgl {

void cdevice::initialize(const RenderWindow& renderWindow)
{
	m_swap_chain.initialize(renderWindow);
	m_pD3DDeviceContext = renderWindow.GetD3D11DeviceContext();


	// TODO: YLAD presents an empty frame here... Yakuza 6 doesn't seem to
	// The game sets private data, then renders a frame, then resets private data
	// For now, just do it directly
	m_context_desc.reset(nullptr, 0.0f, 0, 0);
	void* dataPtr = &m_context_desc;
	m_pD3DDeviceContext->SetPrivateData(ccontext_native::GUID_ContextPrivateData, sizeof(dataPtr), &dataPtr);
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

void ccontext_native::desc_st::reset(ID3D11RenderTargetView* pDepthStencilView, float depth_clear_value, unsigned int width, unsigned int height)
{
	for (size_t i = 0; i < 8; i++)
	{
		m_ppRenderTargetView[i] = nullptr;
		m_p_fast_clear_color[i] = {};
	}
	m_num_slots = 0;
	m_width = width;
	m_height = 0;
	m_depth_clear_value = depth_clear_value;
}

}

void cgs_shader_uniform::initialize()
{
	m_clip_far = 0.0f;
	m_clip_far = 1.0f;
	m_data.m_data_material_modify.saturation = 1.0f;
	m_data.m_data_material_modify.palette0 = -1;
	m_data.m_data_material_modify.palette1 = -1;
	m_data.m_data_material_modify._reserve0 = 1.0f;
}
