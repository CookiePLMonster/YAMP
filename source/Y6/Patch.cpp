#include "Patch.h"

void PatchSl(sl::context_t* context)
{

}

void PatchGs(gs::context_t* context, const RenderWindow& window)
{
	// Fill the export context
	auto& export_context = context->export_context;
	export_context.size_of_struct = sizeof(export_context);
	export_context.sbgl_context.p_value[0] = window.GetD3D11Device();
}