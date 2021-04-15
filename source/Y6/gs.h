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
		float m_depth_clear_value;
		__m128 m_p_fast_clear_color[8];
	};

	static constexpr GUID GUID_ContextPrivateData = { 0x0A84B07F7, 0x3BD0, 0x4C4E, 0x89, 0x90, 0xE9, 0xD5, 0xAF, 0x6E, 0xFB, 0xA2 };
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

}

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

struct alignas(16) cgs_cb
{
	cgs_buffer m_buffer;
	char m_sbgl_resource_buf[32];
	unsigned int m_user_flags;
	unsigned int m_create_flags;
};

struct cgs_cb_pool
{
  cgs_cb_pool* mp_link = nullptr;
  unsigned int m_cb_num = 0;
  cgs_cb *mp_small_tbl[96][32];
  cgs_cb *mp_large_tbl[96][32];
  cgs_cb *mp_huge_tbl[96][32];
};

struct cgs_up_pool
{
	unsigned int m_vb_size;
	unsigned int m_ib_size;
	unsigned int m_vb_ptr;
	unsigned int m_ib_ptr;
	unsigned int m_current_stride;
	unsigned int m_current_vertex_count;
	unsigned int m_current_vertex_offset;
	unsigned int m_current_index_count;
	unsigned int m_current_index_offset;
	unsigned int m_last_frame_counter_vb;
	unsigned int m_last_frame_counter_ib;
	cgs_up_pool* mp_link;
	/*cgs_vb*/void *mp_vb; // We can ask the DLL to create those so there's no need to worry about their structure
	/*cgs_ib*/void *mp_ib; // (for now?)
	void *mp_push_polygon;
	void *mp_push_line;
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
	cgs_device_context* p_device_context;
	sbgl::cdevice sbgl_device;
	std::byte gap2[32];
	t_lockfree_stack<cgs_cb_pool> stack_cb_pool;
	t_lockfree_stack<cgs_up_pool> stack_up_pool;
	t_lockfree_stack<cgs_shader_uniform> stack_shader_uniform;
};
static_assert(offsetof(context_t, p_device_context) == 0xB0);
static_assert(offsetof(context_t, sbgl_device) == 0xC0);
static_assert(offsetof(context_t, sbgl_device.m_pD3DDeviceContext) == 0x150); // Redundant,but validates the assumption
																			  // that m_pD3DDeviceContext
static_assert(offsetof(context_t, stack_cb_pool) == 0x13E0);
static_assert(offsetof(context_t, stack_up_pool) == 0x13E8);
static_assert(offsetof(context_t, stack_shader_uniform) == 0x13F0);

extern context_t* sm_context;

}