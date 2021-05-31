#pragma once

#include <cstdint>
#include <cstddef>

#include <d3d11.h>
#include <xmmintrin.h>

#include "../pxd_shader.h"
#include "../pxd_types.h"

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

class csurface_resource : public csurface
{
public:
	union
	{
		ID3D11Resource *m_pD3DResource;
		ID3D11Texture1D *m_pD3DTexture1D;
		ID3D11Texture2D *m_pD3DTexture2D;
		ID3D11Texture3D *m_pD3DTexture3D;
	};
};

// This is NOT a mistake :/
class ccontext_native : public ID3D11DeviceContext
{
public:
	struct desc_st
	{
		void reset(ID3D11RenderTargetView* pDepthStencilView, float depth_clear_value, unsigned int width, unsigned int height);

		ID3D11RenderTargetView* m_ppRenderTargetView[8];
		ID3D11DepthStencilView* m_pDepthStencilView;
		unsigned int m_num_slots;
		unsigned int m_width;
		unsigned int m_height;
		float m_depth_clear_value; // Probably YLAD only
		//__m128 m_p_fast_clear_color[8]; // YLAD only
	};

	static constexpr GUID GUID_ContextPrivateData = { 0x0A84B07F7, 0x3BD0, 0x4C4E, 0x89, 0x90, 0xE9, 0xD5, 0xAF, 0x6E, 0xFB, 0xA2 };
};
static_assert(sizeof(ccontext_native) == sizeof(ID3D11DeviceContext*)); // If it's not, everything will fall apart
static_assert(sizeof(ccontext_native::desc_st) == 0x58); // TODO: Needs verification, but is almost sure to be correct

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
	//__m128 m_fast_clear_color; // TODO: Educated guess, YLAD only
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
#ifdef _Y6_DAY1_DLL // Removed in a patch
	timestamp_st m_timestamp;
#endif
	ID3D11Query *m_pD3DQuery;
	//__m128 m_fast_clear_color; // YLAD only
	__m128 m_p_border_color[256];
	DXGI_ADAPTER_DESC1 m_DXGIAdapterDesc;
};
static_assert(offsetof(cdevice_native, m_pD3DDeviceContext) == 0x90);
static_assert(offsetof(cdevice_native, m_context_desc) == 0x98);
static_assert(offsetof(cdevice_native, m_mwa_flags) == 0xF4);
static_assert(offsetof(cdevice_native, m_max_frame_latency) == 0xF8);
#ifdef _Y6_DAY1_DLL // TODO: Make this dynamic - this struct changed in a patch
static_assert(offsetof(cdevice_native, m_pD3DQuery) == 0x118);
static_assert(offsetof(cdevice_native, m_p_border_color) == 0x120);
#else
static_assert(offsetof(cdevice_native, m_pD3DQuery) == 0x100);
static_assert(offsetof(cdevice_native, m_p_border_color) == 0x110);
#endif

class cdevice : public cdevice_native
{
public:
	struct shared_symbol_st
	{
		void* p_value[8];
	};

	void initialize(const RenderWindow& renderWindow);
};

struct cbase_target_native : public csurface_resource
{
	union
	{
		ID3D11View *m_pD3DView;
		ID3D11RenderTargetView *m_pD3DRenderTargetView;
		ID3D11DepthStencilView *m_pD3DDepthStencilView;
	};
	//__m128 m_fast_clear_color; // Probably YLAD only
};

struct cbase_target : public cbase_target_native
{
};

struct ccolor_target : public cbase_target
{
};

struct cshader_resource_view
{
	ID3D11ShaderResourceView* m_pD3DShaderResourceView;
};

struct cunordered_access_view
{
	ID3D11UnorderedAccessView* m_pD3DUnorderedAccessView;
};

struct alignas(16) cbase_texture_native : public csurface_resource, cshader_resource_view, cunordered_access_view
{
};

struct cbase_texture : public cbase_texture_native
{
};


struct ctexture : public cbase_texture
{
};

}

struct csbgl_texture_gs : public sbgl::ctexture
{
};

struct csbgl_staging_texture_gs {};
struct csbgl_rw_texture_gs {};

struct csbgl_color_target_gs : public sbgl::ccolor_target
{
};

enum GSFMT
{
	GSFMT_UNKNOWN = 0x0,
	GSFMT_RGBA32_F = 0x81408082,
	GSFMT_RGBA32_U = 0x81400083,
	GSFMT_RGBA32_S = 0x81404084,
	GSFMT_RGB32_F = 0x80308286,
	GSFMT_RGB32_U = 0x80300287,
	GSFMT_RGB32_S = 0x80304288,
	GSFMT_RGBA16_F = 0x8120848A,
	GSFMT_RGBA16_UN = 0x8120C48B,
	GSFMT_RGBA16_U = 0x8120048C,
	GSFMT_RGBA16_SN = 0x8121048D,
	GSFMT_RGBA16_S = 0x8120448E,
	GSFMT_RG32_F = 0x81208790,
	GSFMT_RG32_U = 0x81200791,
	GSFMT_RG32_S = 0x81204792,
	GSFMT_D32_F_S8X24_U = 0x6214994,
	GSFMT_R32_F_X8X24 = 0x218995,
	GSFMT_X32_G8X24_U = 0x21C996,
	GSFMT_RGB10A2_UN = 0x8110CB98,
	GSFMT_RGB10A2_U = 0x81100B99,
	GSFMT_RG11B10_F = 0x81108D1A,
	GSFMT_RGBA8_UN = 0x8110CD9C,
	GSFMT_RGBA8_UN_SRGB = 0x110CD9D,
	GSFMT_RGBA8_U = 0x81100D9E,
	GSFMT_RGBA8_SN = 0x81110D9F,
	GSFMT_RGBA8_S = 0x81104DA0,
	GSFMT_RG16_F = 0x811090A2,
	GSFMT_RG16_UN = 0x8110D0A3,
	GSFMT_RG16_U = 0x811010A4,
	GSFMT_RG16_SN = 0x811110A5,
	GSFMT_RG16_S = 0x811050A6,
	GSFMT_D32_F = 0x21093A8,
	GSFMT_R32_F = 0x811093A9,
	GSFMT_R32_U = 0x811013AA,
	GSFMT_R32_S = 0x811053AB,
	GSFMT_D24_UN_S8_U = 0x612162D,
	GSFMT_R24_UN_X8 = 0x12562E,
	GSFMT_X24_G8_U = 0x11D62F,
	GSFMT_RG8_UN = 0x8108D831,
	GSFMT_RG8_U = 0x81081832,
	GSFMT_RG8_SN = 0x81091833,
	GSFMT_RG8_S = 0x81085834,
	GSFMT_R16_F = 0x81089AB6,
	GSFMT_R16_UN = 0x8108DAB8,
	GSFMT_R16_U = 0x81081AB9,
	GSFMT_R16_SN = 0x81091ABA,
	GSFMT_R16_S = 0x81085ABB,
	GSFMT_D16_UN = 0x208DAB7,
	GSFMT_R8_UN = 0x8104DE3D,
	GSFMT_R8_U = 0x8104DE3E,
	GSFMT_R8_SN = 0x8104DE3F,
	GSFMT_R8_S = 0x8104DE40,
	GSFMT_A8_UN = 0x104E0C1,
	GSFMT_RGB9E5_SE = 0x12A1C3,
	GSFMT_RG8BG8_UN = 0x10E244,
	GSFMT_GR8GB8_UN = 0x10E2C5,
	GSFMT_BC1_UN = 0xA0E347,
	GSFMT_BC1_UN_SRGB = 0xA0E348,
	GSFMT_BC2_UN = 0xC0E4CA,
	GSFMT_BC2_UN_SRGB = 0xC0E4CB,
	GSFMT_BC3_UN = 0xC0E64D,
	GSFMT_BC3_UN_SRGB = 0xC0E64E,
	GSFMT_BC4_UN = 0xA0E7D0,
	GSFMT_BC4_SN = 0xA127D1,
	GSFMT_BC5_UN = 0xC0E953,
	GSFMT_BC5_SN = 0xC12954,
	GSFMT_BC6H_UF = 0xC2EF5F,
	GSFMT_BC6H_SF = 0xC32F60,
	GSFMT_BC7_UN = 0xC0F0E2,
	GSFMT_BC7_UN_SRGB = 0xC0F0E3,
	GSFMT_B5G6R5_UN = 0x8EAD5,
	GSFMT_BGR5A1_UN = 0x8EB56,
	GSFMT_BGRA8_UN = 0x8110ED57,
	GSFMT_BGRA8_UN_SRGB = 0x110ED5B,
	GSFMT_BGR8X8_UN = 0x8010EE58,
	GSFMT_BGR8X8_UN_SRGB = 0x10EE5D,
	GSFMT_XYZW32_F = 0x81408082,
	GSFMT_XYZW32_U = 0x81400083,
	GSFMT_XYZW32_S = 0x81404084,
	GSFMT_XYZ32_F = 0x80308286,
	GSFMT_XYZ32_U = 0x80300287,
	GSFMT_XYZ32_S = 0x80304288,
	GSFMT_XYZW16_F = 0x8120848A,
	GSFMT_XYZW16_UN = 0x8120C48B,
	GSFMT_XYZW16_U = 0x8120048C,
	GSFMT_XYZW16_SN = 0x8121048D,
	GSFMT_XYZW16_S = 0x8120448E,
	GSFMT_XY32_F = 0x81208790,
	GSFMT_XY32_U = 0x81200791,
	GSFMT_XY32_S = 0x81204792,
	GSFMT_XYZW8_UN = 0x8110CD9C,
	GSFMT_XYZW8_U = 0x81100D9E,
	GSFMT_XYZW8_SN = 0x81110D9F,
	GSFMT_XYZW8_S = 0x81104DA0,
	GSFMT_XY16_F = 0x811090A2,
	GSFMT_XY16_UN = 0x8110D0A3,
	GSFMT_XY16_U = 0x811010A4,
	GSFMT_XY16_SN = 0x811110A5,
	GSFMT_XY16_S = 0x811050A6,
	GSFMT_X32_F = 0x811093A9,
	GSFMT_X32_U = 0x811013AA,
	GSFMT_X32_S = 0x811053AB,
	GSFMT_X32Y32Z32W32F = 0x81408082,
	GSFMT_X32Y32Z32W32U = 0x81400083,
	GSFMT_X32Y32Z32W32S = 0x81404084,
	GSFMT_X32Y32Z32F = 0x80308286,
	GSFMT_X32Y32Z32U = 0x80300287,
	GSFMT_X32Y32Z32S = 0x80304288,
	GSFMT_X32Y32F = 0x81208790,
	GSFMT_X32Y32U = 0x81200791,
	GSFMT_X32Y32S = 0x81204792,
	GSFMT_X32F = 0x811093A9,
	GSFMT_X32U = 0x811013AA,
	GSFMT_X32S = 0x811053AB,
	GSFMT_X16Y16Z16W16F = 0x8120848A,
	GSFMT_X16Y16F = 0x811090A2,
	GSFMT_B8G8R8A8 = 0x8110ED57,
	GSFMT_R8G8B8A8 = 0x8110CD9C,
	GSFMT_A8 = 0x104E0C1,
	GSFMT_R10G10B10A2 = 0x8110CB98,
	GSFMT_R11G11B10F = 0x81108D1A,
	GSFMT_R32F = 0x811093A9,
	GSFMT_R16G16B16A16F = 0x8120848A,
	GSFMT_R32G32B32A32F = 0x81408082,
	GSFMT_U16V16 = 0x811110A5,
	GSFMT_R16G16 = 0x8110D0A3,
	GSFMT_R16G16F = 0x811090A2,
	GSFMT_D32F = 0x21093A8,
	GSFMT_D16 = 0x208DAB7,
	GSFMT_D24S8 = 0x612162D,
	GSFMT_D32FS8 = 0x6214994,
	GSFMT_B8G8R8X8 = 0x8010EE58,
	GSFMT_B5G6R5 = 0x8EAD5,
	GSFMT_B5G5R5A1 = 0x8EB56,
	GSFMT_BC1 = 0xA0E347,
	GSFMT_BC2 = 0xC0E4CA,
	GSFMT_BC3 = 0xC0E64D,
	GSFMT_BC4U = 0xA0E7D0,
	GSFMT_BC4S = 0xA127D1,
	GSFMT_BC5U = 0xC0E953,
	GSFMT_BC5S = 0xC12954,
	GSFMT_BC6HUF = 0xC2EF5F,
	GSFMT_BC6HSF = 0xC32F60,
	GSFMT_BC7 = 0xC0F0E2,
	GSFMT_B8G8R8A8_SRGB = 0x110ED5B,
	GSFMT_R8G8B8A8_SRGB = 0x110CD9D,
	GSFMT_BC1_SRGB = 0xA0E348,
	GSFMT_BC2_SRGB = 0xC0E4CB,
	GSFMT_BC3_SRGB = 0xC0E64E,
	GSFMT_BC7_SRGB = 0xC0F0E3,
	GSFMT_DXT1 = 0xA0E347,
	GSFMT_DXT2 = 0xC0E4CA,
	GSFMT_DXT3 = 0xC0E4CA,
	GSFMT_DXT4 = 0xC0E64D,
	GSFMT_DXT5 = 0xC0E64D,
	GSFMT_X1R5G5B5 = 0x8EB56,
	GSFMT_D24X8 = 0x612162D,
	GSFMT_L16 = 0x8108DAB8,
	GSFMT_A8R8G8B8 = 0x8110ED57,
	GSFMT_A1R5G5B5 = 0x8EB56,
	GSFMT_A16B16G16R16F = 0x8120848A,
	GSFMT_A32B32G32R32F = 0x81408082,
	GSFMT_V16U16 = 0x811110A5,
	GSFMT_G16R16 = 0x8110D0A3,
	GSFMT_G16R16F = 0x811090A2,
	GSFMT_X8R8G8B8 = 0x8010EE58,
	GSFMT_R5G6B5 = 0x8EAD5,
	GSFMT_R5G5B5A1 = 0x8EB56,
	GSFMT_DEPTH_D32F = 0x811093A9,
	GSFMT_DEPTH_D16 = 0x8108DAB8,
	GSFMT_DEPTH_D24S8 = 0x12562E,
	GSFMT_DEPTH_D32FS8 = 0x218995,
	GSFMT_C8 = 0x8104DE3D,
	GSFMT_A8L8 = 0x8108D831,
	GSFMT_L8 = 0x8104DE3D,
	GSFMT_FORCE_32BIT = 0x7FFFFFFF,
};

struct cgs_buffer
{
	volatile unsigned int m_ref_count;
	rwspinlock_t m_sync;
	unsigned int m_usage;
	unsigned int m_size;
	union
	{
		struct
		{
			uint64_t m_this : 48;
			uint64_t m_counter : 16;
		};
		uint64_t m_uid;
	} uid;
	union
	{
		/*sbgl::cbase_buffer*/void *mp_sbgl_resource;
		/*csbgl_staging_buffer_gs*/void *mp_sbgl_staging_resource;
		/*csbgl_vertex_buffer_gs*/void *mp_sbgl_vertex_buffer;
		/*csbgl_index_buffer_gs*/void *mp_sbgl_index_buffer;
		/*csbgl_constant_buffer_gs*/void *mp_sbgl_constant_buffer;
		/*csbgl_rw_buffer_gs*/void *mp_sbgl_rw_buffer;
		/*csbgl_ro_buffer_gs*/void *mp_sbgl_ro_buffer;
		/*csbgl_buffer_gs*/void *mp_sbgl_base_buffer;
	} buffer;
};

struct cgs_rt
{
	std::byte gap[36];
	csbgl_color_target_gs* mp_sbgl_resource;
};
static_assert(sizeof(cgs_rt) == 48);
static_assert(offsetof(cgs_rt, mp_sbgl_resource) == 0x28);

struct cgs_depth {};
struct cgs_async_resource_tex {};

struct alignas(16) cgs_cb
{
	cgs_buffer m_buffer;
	char m_sbgl_resource_buf[32];
	unsigned int m_user_flags;
	unsigned int m_create_flags;
};

class alignas(8) cgs_tex
{
public:
	volatile unsigned int m_ref_count = 1;
	recursive_rwspinlock_t m_sync;
	unsigned int m_id;
	unsigned int m_usage;
	GSFMT m_fmt;
	unsigned int m_create_flags;
	unsigned __int32 m_width : 15;
	unsigned __int32 m_depth : 13;
	unsigned __int32 m_mip_levels : 4;
	unsigned __int32 m_height : 15;
	unsigned __int32 m_array_slices : 13;
	unsigned __int32 m_type : 4;
	union
	{
		cgs_rt* mp_rt;
		cgs_depth* mp_depth;
		cgs_async_resource_tex* mp_async_resource;
	};
	union
	{
		csbgl_texture_gs* mp_sbgl_resource;
		csbgl_staging_texture_gs* mp_sbgl_staging_resource;
		csbgl_rw_texture_gs* mp_sbgl_rw_resource;
	};
	t_avl_tree_node<cgs_tex> m_node;
	csl_hash m_name;
	//unsigned int m_gnm_resource; // YLAD only
};
static_assert(sizeof(cgs_tex) == 96);

// We can ask the DLL to create those so there's no need to worry about their structure
// (for now?)
struct alignas(8) cgs_vb {};
struct cgs_ib {};
struct cgs_fx {};
struct cgs_mesh {};
struct cgs_vs {};
struct cgs_ps {};
struct cgs_gs {};
struct cgs_hs {};
struct cgs_ds {};
struct cgs_cs {};
struct cgs_gts {};

struct cgs_cb_pool
{
  cgs_cb_pool* mp_link = nullptr;
  unsigned int m_cb_num = 0;
  cgs_cb *mp_small_tbl[96][32] {};
  cgs_cb *mp_large_tbl[96][32] {};
  cgs_cb *mp_huge_tbl[96][32] {};
};

struct cgs_up_pool
{
public:
	void initialize(unsigned int vb_size, unsigned int ib_size, bool is_push_support);

public: // TODO: Make private, mp_link requires public for now
	unsigned int m_vb_size = 0;
	unsigned int m_ib_size = 0;
	unsigned int m_vb_ptr = 0;
	unsigned int m_ib_ptr = 0;
	unsigned int m_current_stride = 0;
	unsigned int m_current_vertex_count = 0;
	unsigned int m_current_vertex_offset = 0;
	unsigned int m_current_index_count = 0;
	unsigned int m_current_index_offset = 0;
	unsigned int m_last_frame_counter_vb = 0;
	unsigned int m_last_frame_counter_ib = 0;
	cgs_up_pool* mp_link = nullptr;
	cgs_vb* mp_vb = nullptr;
	cgs_ib* mp_ib = nullptr;
	void *mp_push_polygon = nullptr;
	void *mp_push_line = nullptr;
};
static_assert(sizeof(cgs_up_pool) == 88);

class cgs_shader_uniform
{
public:
	void initialize();

public:
	cgs_shader_uniform* mp_link = nullptr;
	uint64_t m_dirty_status[2] {};
	uint64_t m_uniform_status[94];
	float m_clip_near;
	float m_clip_far;
	// Yakuza 6 doesn't have extend data, YLAD does
	//unsigned int m_extend_data_size;
	//void (*m_extend_data_reset_callback)(cgs_shader_uniform*, void*);
	matrix m_mtx_world;
	matrix m_mtx_view;
	matrix m_mtx_projection;
	matrix m_mtx_view_projection;
	uniform_struct_base_t m_data;
};
static_assert(sizeof(cgs_shader_uniform) == 17200);

class cgs_device_context
{
public:
	void initialize(sbgl::ccontext* p_context);
	void reset_state_all()
	{
		reset_state_all_internal(this);
	}

	uint64_t m_dirty_state;
	uint64_t m_dirty_state_gts;
	uint64_t m_dirty_state_cs;
	sbgl::ccontext* mp_sbgl_context;
	/*csbgl_deferred_context*/void* mp_sbgl_deferred_context;
	cgs_cb_pool* mp_cb_pool;
	cgs_up_pool* mp_up_pool;
	cgs_shader_uniform* mp_shader_uniform;
	std::byte gap[7882];

	static inline void (__thiscall *reset_state_all_internal)(cgs_device_context* obj);
};
static_assert(sizeof(cgs_device_context) == 7952);
static_assert(offsetof(cgs_device_context, mp_sbgl_context) == 24);
static_assert(offsetof(cgs_device_context, mp_shader_uniform) == 56);

namespace gs {

enum vb_usage_t
{
	VB_USAGE_DEFAULT = 0x120,
	VB_USAGE_IMMUTABLE = 0x101,
	VB_USAGE_DYNAMIC = 0x10A,
	VB_USAGE_STAGING = 0x3B,
};

enum ib_usage_t
{
	IB_USAGE_DEFAULT = 0x220,
	IB_USAGE_IMMUTABLE = 0x201,
	IB_USAGE_DYNAMIC = 0x20A,
	IB_USAGE_STAGING = 0x3B,
};

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
	unsigned int frame_counter;
	std::byte gap[76];
	cgs_device_context* p_device_context;
	sbgl::cdevice sbgl_device;
	std::byte gap2[32];
	cgs_vb *p_vb_sphere[3];
	cgs_vb *p_vb_capsule[3];
	cgs_ib *p_ib_quad;
	cgs_ib *p_ib_fan;
	//cgs_ib *p_ib_rect; // YLAD only
	std::byte gap3[96];
	t_lockfree_stack<cgs_cb_pool> stack_cb_pool;
	t_lockfree_stack<cgs_up_pool> stack_up_pool;
	t_lockfree_stack<cgs_shader_uniform> stack_shader_uniform;
	std::byte gap4[808];
	t_instance_tbl<cgs_mesh> handle_mesh;
	t_instance_tbl<cgs_tex> handle_tex;
	t_instance_tbl<cgs_vs> handle_vs;
	t_instance_tbl<cgs_ps> handle_ps;
	t_instance_tbl<cgs_gs> handle_gs;
	t_instance_tbl<cgs_hs> handle_hs;
	t_instance_tbl<cgs_ds> handle_ds;
	t_instance_tbl<cgs_cs> handle_cs;
	t_instance_tbl<cgs_gts> handle_gts;
	t_instance_tbl<cgs_fx> handle_fx;
};
static_assert(offsetof(context_t, frame_counter) == 0x60);
static_assert(offsetof(context_t, p_device_context) == 0xB0);
static_assert(offsetof(context_t, sbgl_device) == 0xC0);
static_assert(offsetof(context_t, sbgl_device.m_pD3DDeviceContext) == 0x150); // Redundant,but validates the assumption
																			  // that m_pD3DDeviceContext

// TODO: Make dynamic
#ifdef _Y6_DAY1_DLL
static_assert(offsetof(context_t, p_ib_quad) == 0x1370);
static_assert(offsetof(context_t, p_ib_fan) == 0x1378);
static_assert(offsetof(context_t, stack_cb_pool) == 0x13E0);
static_assert(offsetof(context_t, stack_up_pool) == 0x13E8);
static_assert(offsetof(context_t, stack_shader_uniform) == 0x13F0);
static_assert(offsetof(context_t, handle_tex) == 0x1740);
static_assert(offsetof(context_t, handle_fx) == 0x1840);
#else
static_assert(offsetof(context_t, p_ib_quad) == 0x1360);
static_assert(offsetof(context_t, p_ib_fan) == 0x1368);
static_assert(offsetof(context_t, stack_cb_pool) == 0x13D0);
static_assert(offsetof(context_t, stack_up_pool) == 0x13D8);
static_assert(offsetof(context_t, stack_shader_uniform) == 0x13E0);
static_assert(offsetof(context_t, handle_tex) == 0x1730);
static_assert(offsetof(context_t, handle_fx) == 0x1830);
#endif

void primitive_initialize();

extern cgs_vb* (*vb_create)(uint64_t fvf, unsigned int vertices, vb_usage_t usage, unsigned int flags, const void* p_initial_data, const char* sz_name);
extern cgs_ib* (*ib_create)(unsigned int indices, ib_usage_t usage, unsigned int flags, const void* p_initial_data, const char* sz_name);

extern context_t* sm_context;

}