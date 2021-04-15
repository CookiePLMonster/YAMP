#pragma once

#include <cstdint>
#include <xmmintrin.h>

struct matrix
{
	__m128 m_vm0;
	__m128 m_vm1;
	__m128 m_vm2;
	__m128 m_vm3;
};

struct alignas(16) pxd_shader_bool_t
{
	unsigned int c_bool;
};

struct pxd_shader_cb12_t
{
	matrix c_matP;
	matrix c_matV;
	matrix c_matVP;
	matrix c_matT[4];
	__m128 c_rvtx_param;
	__m128 c_rt_size;
	__m128 c_location;
};

struct pxd_shader_cb10b_t
{
	__m128 c_matW[3];
};

struct pxd_shader_cb10_t
{
	__m128 c_matTBL[256][3];
};

struct pxd_shader_sys_material_t
{
	union
	{
		__m128 diffuse;
		union
		{
			float diffuse_r;
			float diffuse_g;
			float diffuse_b;
			float diffuse_a;
		} comps;
	} diffuse;
	union
	{
		__m128 specular;
		union
		{
			float specular_r;
			float specular_g;
			float specular_b;
			float specular_power;
		} comps;
	} specular;
	union
	{
		__m128 emissive_ambient;
		union
		{
			float emissive_r;
			float emissive_g;
			float emissive_b;
			float ambient;
		} comps;
	} emissive_ambient;
	__m128 scene_ambient;
	__m128 scene_light_vector[2];
	__m128 scene_light_color[2];
};

struct pxd_shader_material_anim_t
{
	union
	{
		union
		{
			float uv_offset[3][2];
			float blend_scale;
			float opacity_scale;
		} comps;
		__m128 v[2];
		uint64_t u[4];
	} anim;
};

struct pxd_shader_material_modify_base_t
{
	union
	{
		union
		{
			float diffuse_scale[3];
			float global_opacity;
			float specular_scale[3];
			float emissive_scale;
		} comps;
		__m128 v[2];
		uint64_t u[4];
	} modify;
};

struct pxd_shader_material_modify_t : public pxd_shader_material_modify_base_t
{
	float saturation;
	unsigned int palette0;
	unsigned int palette1;
	float _reserve0;
};

struct pxd_shader_user_t
{
	__m128 c_user_param[32];
};

struct uniform_struct_base_t
{
	pxd_shader_bool_t m_data_bool_vs;
	pxd_shader_bool_t m_data_bool_ps;
	pxd_shader_bool_t m_data_bool_gs;
	pxd_shader_bool_t m_data_bool_hs;
	pxd_shader_bool_t m_data_bool_ds;
	pxd_shader_bool_t m_data_bool_cs;
	pxd_shader_cb12_t m_data_cb12;
	union
	{
		pxd_shader_cb10b_t m_data_cb10b;
		pxd_shader_cb10_t m_data_cb10;
	} cb10;
	pxd_shader_sys_material_t m_data_sys_material;
	pxd_shader_material_anim_t m_data_material_anim;
	pxd_shader_material_modify_t m_data_material_modify;
	pxd_shader_user_t m_data_user_vs;
	pxd_shader_user_t m_data_user_ps;
	pxd_shader_user_t m_data_user_gs;
	pxd_shader_user_t m_data_user_hs;
	pxd_shader_user_t m_data_user_ds;
	pxd_shader_user_t m_data_user_cs;
};