#pragma once

#include <cstdint>
#include <cstddef>

#include <d3d11.h>
#include <xmmintrin.h>

class RenderWindow;

enum sbgl_format_t
{
	SBGL_FORMAT_UNKNOWN = 0x0,
	SBGL_FORMAT_RGBA32_F = 0x81408082,
	SBGL_FORMAT_RGBA32_U = 0x81400083,
	SBGL_FORMAT_RGBA32_S = 0x81404084,
	SBGL_FORMAT_RGB32_F = 0x80308286,
	SBGL_FORMAT_RGB32_U = 0x80300287,
	SBGL_FORMAT_RGB32_S = 0x80304288,
	SBGL_FORMAT_RGBA16_F = 0x8120848A,
	SBGL_FORMAT_RGBA16_UN = 0x8120C48B,
	SBGL_FORMAT_RGBA16_U = 0x8120048C,
	SBGL_FORMAT_RGBA16_SN = 0x8121048D,
	SBGL_FORMAT_RGBA16_S = 0x8120448E,
	SBGL_FORMAT_RG32_F = 0x81208790,
	SBGL_FORMAT_RG32_U = 0x81200791,
	SBGL_FORMAT_RG32_S = 0x81204792,
	SBGL_FORMAT_D32_F_S8X24_U = 0x6214994,
	SBGL_FORMAT_R32_F_X8X24 = 0x218995,
	SBGL_FORMAT_X32_G8X24_U = 0x21C996,
	SBGL_FORMAT_RGB10A2_UN = 0x8110CB98,
	SBGL_FORMAT_RGB10A2_U = 0x81100B99,
	SBGL_FORMAT_RG11B10_F = 0x81108D1A,
	SBGL_FORMAT_RGBA8_UN = 0x8110CD9C,
	SBGL_FORMAT_RGBA8_UN_SRGB = 0x110CD9D,
	SBGL_FORMAT_RGBA8_U = 0x81100D9E,
	SBGL_FORMAT_RGBA8_SN = 0x81110D9F,
	SBGL_FORMAT_RGBA8_S = 0x81104DA0,
	SBGL_FORMAT_RG16_F = 0x811090A2,
	SBGL_FORMAT_RG16_UN = 0x8110D0A3,
	SBGL_FORMAT_RG16_U = 0x811010A4,
	SBGL_FORMAT_RG16_SN = 0x811110A5,
	SBGL_FORMAT_RG16_S = 0x811050A6,
	SBGL_FORMAT_D32_F = 0x21093A8,
	SBGL_FORMAT_R32_F = 0x811093A9,
	SBGL_FORMAT_R32_U = 0x811013AA,
	SBGL_FORMAT_R32_S = 0x811053AB,
	SBGL_FORMAT_D24_UN_S8_U = 0x612162D,
	SBGL_FORMAT_R24_UN_X8 = 0x12562E,
	SBGL_FORMAT_X24_G8_U = 0x11D62F,
	SBGL_FORMAT_RG8_UN = 0x8108D831,
	SBGL_FORMAT_RG8_U = 0x81081832,
	SBGL_FORMAT_RG8_SN = 0x81091833,
	SBGL_FORMAT_RG8_S = 0x81085834,
	SBGL_FORMAT_R16_F = 0x81089AB6,
	SBGL_FORMAT_R16_UN = 0x8108DAB8,
	SBGL_FORMAT_R16_U = 0x81081AB9,
	SBGL_FORMAT_R16_SN = 0x81091ABA,
	SBGL_FORMAT_R16_S = 0x81085ABB,
	SBGL_FORMAT_D16_UN = 0x208DAB7,
	SBGL_FORMAT_R8_UN = 0x8104DE3D,
	SBGL_FORMAT_R8_U = 0x8104DE3E,
	SBGL_FORMAT_R8_SN = 0x8104DE3F,
	SBGL_FORMAT_R8_S = 0x8104DE40,
	SBGL_FORMAT_A8_UN = 0x104E0C1,
	SBGL_FORMAT_RGB9E5_SE = 0x12A1C3,
	SBGL_FORMAT_RG8BG8_UN = 0x10E244,
	SBGL_FORMAT_GR8GB8_UN = 0x10E2C5,
	SBGL_FORMAT_BC1_UN = 0xA0E347,
	SBGL_FORMAT_BC1_UN_SRGB = 0xA0E348,
	SBGL_FORMAT_BC2_UN = 0xC0E4CA,
	SBGL_FORMAT_BC2_UN_SRGB = 0xC0E4CB,
	SBGL_FORMAT_BC3_UN = 0xC0E64D,
	SBGL_FORMAT_BC3_UN_SRGB = 0xC0E64E,
	SBGL_FORMAT_BC4_UN = 0xA0E7D0,
	SBGL_FORMAT_BC4_SN = 0xA127D1,
	SBGL_FORMAT_BC5_UN = 0xC0E953,
	SBGL_FORMAT_BC5_SN = 0xC12954,
	SBGL_FORMAT_BC6H_UF = 0xC2EF5F,
	SBGL_FORMAT_BC6H_SF = 0xC32F60,
	SBGL_FORMAT_BC7_UN = 0xC0F0E2,
	SBGL_FORMAT_BC7_UN_SRGB = 0xC0F0E3,
	SBGL_FORMAT_B5G6R5_UN = 0x8EAD5,
	SBGL_FORMAT_B5G5R5A1_UN = 0x8EB56,
	SBGL_FORMAT_BGRA8_UN = 0x8110ED57,
	SBGL_FORMAT_BGRA8_UN_SRGB = 0x110ED5B,
	SBGL_FORMAT_BGR8X8_UN = 0x8010EE58,
	SBGL_FORMAT_BGR8X8_UN_SRGB = 0x10EE5D,
	SBGL_FORMAT_XYZW32_F = 0x81408082,
	SBGL_FORMAT_XYZW32_U = 0x81400083,
	SBGL_FORMAT_XYZW32_S = 0x81404084,
	SBGL_FORMAT_XYZ32_F = 0x80308286,
	SBGL_FORMAT_XYZ32_U = 0x80300287,
	SBGL_FORMAT_XYZ32_S = 0x80304288,
	SBGL_FORMAT_XYZW16_F = 0x8120848A,
	SBGL_FORMAT_XYZW16_UN = 0x8120C48B,
	SBGL_FORMAT_XYZW16_U = 0x8120048C,
	SBGL_FORMAT_XYZW16_SN = 0x8121048D,
	SBGL_FORMAT_XYZW16_S = 0x8120448E,
	SBGL_FORMAT_XY32_F = 0x81208790,
	SBGL_FORMAT_XY32_U = 0x81200791,
	SBGL_FORMAT_XY32_S = 0x81204792,
	SBGL_FORMAT_XYZW8_UN = 0x8110CD9C,
	SBGL_FORMAT_XYZW8_U = 0x81100D9E,
	SBGL_FORMAT_XYZW8_SN = 0x81110D9F,
	SBGL_FORMAT_XYZW8_S = 0x81104DA0,
	SBGL_FORMAT_XY16_F = 0x811090A2,
	SBGL_FORMAT_XY16_UN = 0x8110D0A3,
	SBGL_FORMAT_XY16_U = 0x811010A4,
	SBGL_FORMAT_XY16_SN = 0x811110A5,
	SBGL_FORMAT_XY16_S = 0x811050A6,
	SBGL_FORMAT_X32_F = 0x811093A9,
	SBGL_FORMAT_X32_U = 0x811013AA,
	SBGL_FORMAT_X32_S = 0x811053AB,
	SBGL_FORMAT_FORCE_32BIT = 0x7FFFFFFF,
};

// gs definitions for Yakuza 6
namespace sbgl {

class alignas(8) csurface
{
public:
	struct base_dimension_st
	{
		uint32_t width : 16;
		uint32_t height : 16;
		uint32_t depth : 16;
	};

	union
	{
		unsigned int m_desc;
		struct
		{
			uint32_t m_width_minus_1 : 14;
			uint32_t m_height_minus_1 : 14;
			uint32_t m_mip_levels : 4;
		} d;
	} desc1;
	union
	{
		unsigned int m_desc;
		struct
		{
			uint32_t m_depth_minus_1 : 11;
			uint32_t m_array_slices_minus_1 : 11;
			uint32_t m_ms_count_log2 : 3;
			uint32_t m_ms_quality : 5;
			uint32_t m_overlap_level : 1;		
		} d;
	} desc2;
	sbgl_format_t m_format;
	unsigned int m_flags;
	sbgl::csurface::base_dimension_st m_base_dimension;
	float m_depth_clear_value;
};

// This is NOT a mistake :/
class ccontext_native : public ID3D11DeviceContext
{
public:
	struct desc_st
	{
		ID3D11RenderTargetView* m_ppRenderTargetView[8];
		ID3D11DepthStencilView* m_pDepthStencilView;
		unsigned int m_num_slots;
		unsigned int m_width;
		unsigned int m_height;
		float m_depth_clear_value;
		__m128 m_p_fast_clear_color[8];
	};
};
static_assert(sizeof(ccontext_native) == sizeof(ID3D11DeviceContext*)); // If it's not, everything will fall apart

class ccontext : public ccontext_native
{
};

class alignas(16) cswap_chain_common
{
public:
	HRESULT initialize(const RenderWindow& window);

public:
	struct window_st
	{
		HWND* hwnd;
		unsigned int style;
		unsigned int style_ex;
		unsigned int width;
		unsigned int height;
	};

	IDXGISwapChain *m_pDXGISwapChain;
	ID3D11RenderTargetView *m_pD3DRenderTargetView;
	ID3D11DepthStencilView *m_pD3DDepthStencilView;
	ID3D11Texture2D *m_pD3DDepthStencilBuffer;
	sbgl::csurface m_color_surface;
	sbgl::csurface m_depth_surface;
	unsigned int m_buffer_index;
	unsigned __int32 m_num_buffers : 8;
	unsigned __int32 m_type : 8;
	//__m128 m_fast_clear_color; // TODO: Educated guess
	std::byte gap[16];
	sbgl::cswap_chain_common::window_st m_window;
};
static_assert(sizeof(cswap_chain_common) == 0x90); // TODO: Educated guess

class alignas(16) cdevice_native
{
public:
	struct timestamp_st
	{
		ID3D11Query *pD3DQuery;
		int64_t frequency;
		unsigned __int32 disjoint : 8;
		unsigned __int32 state : 8;
	};

	cswap_chain_common m_swap_chain;
	ID3D11DeviceContext *m_pD3DDeviceContext;
	ccontext_native::desc_st m_context_desc;
	D3D_FEATURE_LEVEL m_FeatureLevelSupported;
	unsigned int m_mwa_flags;
	unsigned int m_max_frame_latency;
	timestamp_st m_timestamp;
	ID3D11Query *m_pD3DQuery;
	__m128 m_fast_clear_color;
	__m128 m_p_border_color[256];
	DXGI_ADAPTER_DESC1 m_DXGIAdapterDesc;
};
static_assert(offsetof(cdevice_native, m_pD3DDeviceContext) == 0x90);

class cdevice : public cdevice_native
{
public:
	struct shared_symbol_st
	{
		void* p_value[8];
	};

	void initialize(const RenderWindow& renderWindow);
};

class cgs_device_context
{
public:
	void initialize(ccontext* p_context);

	std::byte gap2[24];
	ccontext* mp_sbgl_context;
	std::byte gap[7920];
};
static_assert(sizeof(cgs_device_context) == 7952);

}

namespace gs {

struct export_context_t
{
	size_t size_of_struct;
	void* p_context;
	sbgl::cdevice::shared_symbol_st sbgl_context;
};

struct context_t
{
  uint32_t tag_id; // 0x7367424C
  uint32_t version; // 0x40601
  uint32_t size_of_struct; // Should be 8128 when complete
  uint32_t sbgl_initialize_flags;
  export_context_t export_context;
  std::byte gap[80];
  sbgl::cgs_device_context* p_device_context;
  sbgl::cdevice sbgl_device;
};
static_assert(offsetof(context_t, p_device_context) == 0xB0);
static_assert(offsetof(context_t, sbgl_device) == 0xC0);
static_assert(offsetof(context_t, sbgl_device.m_pD3DDeviceContext) == 0x150); // Redundant,but validates the assumption
																			  // that m_pD3DDeviceContext

extern context_t* sm_context;

}