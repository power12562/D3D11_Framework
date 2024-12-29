#include "PBRDirectionalLight.h"
#include <D3DCore/D3DConstBuffer.h>

namespace DirectionalLight
{
    std::shared_ptr<cb_PBRDirectionalLight> DirectionalLights = D3DConstBuffer::GetData<cb_PBRDirectionalLight>(DirectionalLights_key);
};

void cb_PBRDirectionalLight::PushLight()
{
    if (LightsCount < MAX_LIGHT_COUNT)
    {
        ++LightsCount;
    }
}

void cb_PBRDirectionalLight::PopLight()
{
    if (LightsCount > 0)
    {
        --LightsCount;
    }
}
