#include "SimpleDirectionalLight.h"
#include <D3DCore/D3DConstBuffer.h>

namespace SimpleDirectionalLight
{
    std::shared_ptr<cb_DirectionalLight> cb_light = D3DConstBuffer::GetData<cb_DirectionalLight>(cb_light_key);
};