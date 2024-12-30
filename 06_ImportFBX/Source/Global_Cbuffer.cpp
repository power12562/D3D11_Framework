#include "Global_Cbuffer.h"
#include <D3DCore/D3DConstBuffer.h>

namespace Global_Cbuffer
{
    std::shared_ptr<cbuffer_bool> cb_bool = D3DConstBuffer::GetData<cbuffer_bool>(cb_bool_key);
    std::shared_ptr<cb_BlingPhongMaterial> cb_material = D3DConstBuffer::GetData<cb_BlingPhongMaterial>(cb_material_key);
};