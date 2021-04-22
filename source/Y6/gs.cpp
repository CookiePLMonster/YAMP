#include "gs.h"

#include "../RenderWindow.h"

namespace gs {

cgs_vb* (*vb_create)(uint64_t fvf, unsigned int vertices, vb_usage_t usage, unsigned int flags, const void* p_initial_data, const char* sz_name);
cgs_ib* (*ib_create)(unsigned int indices, ib_usage_t usage, unsigned int flags, const void* p_initial_data, const char* sz_name);
context_t* sm_context;

void primitive_initialize()
{
	context_t* context = sm_context;

	for (size_t i = 0; i < 3; i++)
	{
		context->p_vb_sphere[i] = nullptr;
		context->p_vb_capsule[i] = nullptr;
	}

	{
		constexpr size_t NUM_PRIMITIVES = 192;
		uint16_t verts[NUM_PRIMITIVES][6];
		uint16_t primNum = 0;
		for (auto& vert : verts)
		{
			const uint16_t startVertNum = 4 * primNum++;
			size_t idx = 0;
			vert[idx++] = startVertNum;
			vert[idx++] = startVertNum + 1;
			vert[idx++] = startVertNum + 2;
			vert[idx++] = startVertNum;
			vert[idx++] = startVertNum + 2;
			vert[idx++] = startVertNum + 3;
		}
		context->p_ib_quad = ib_create(NUM_PRIMITIVES * 6, IB_USAGE_IMMUTABLE, 0, verts, nullptr);
	}
	{
		constexpr size_t NUM_PRIMITIVES = 256;
		uint16_t verts[NUM_PRIMITIVES][3];
		uint16_t primNum = 0;
		for (auto& vert : verts)
		{
			const uint16_t startVertexNum = primNum++;
			size_t idx = 0;
			vert[idx++] = startVertexNum + 2;
			vert[idx++] = 0;
			vert[idx++] = startVertexNum + 1;
		}
		// BUG: Original code creates a IB 2x bigger, but this seems like a bug
		context->p_ib_fan = ib_create(NUM_PRIMITIVES * 3, IB_USAGE_IMMUTABLE, 0, verts, nullptr);
	}
}

}

void cgs_device_context::initialize(sbgl::ccontext* p_context)
{
	if (p_context != nullptr)
	{
		mp_sbgl_context = p_context;

		mp_cb_pool = gs::sm_context->stack_cb_pool.pop();
		mp_up_pool = gs::sm_context->stack_up_pool.pop();
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

	std::fill(std::begin(m_p_border_color), std::end(m_p_border_color), _mm_set1_ps(std::numeric_limits<float>::quiet_NaN()));
	m_p_border_color[0] = _mm_setzero_ps();
	m_p_border_color[1] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	m_p_border_color[2] = _mm_set1_ps(1.0f);

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
		//m_p_fast_clear_color[i] = {}; // YLAD only
	}
	m_num_slots = 0;
	m_width = width;
	m_height = 0;
	m_depth_clear_value = depth_clear_value;
}

}

void cgs_up_pool::initialize(unsigned int vb_size, unsigned int ib_size, bool is_push_support)
{
	m_vb_size = (vb_size + 63) & ~63;
	m_ib_size = (ib_size + 63) & ~63;

	if (m_vb_size != 0)
	{
		mp_vb = gs::vb_create(0xE00003u, m_vb_size / 16, gs::VB_USAGE_DYNAMIC, 0, nullptr, nullptr);
	}
	if (m_ib_size != 0)
	{
		mp_ib = gs::ib_create(m_ib_size / 2, gs::IB_USAGE_DYNAMIC, 0, nullptr, nullptr);
	}
	if (is_push_support)
	{
		char* buf = static_cast<char*>(::operator new(2 * m_vb_size, std::align_val_t(16)));
		mp_push_polygon = buf;
		mp_push_line = buf+m_vb_size;
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
