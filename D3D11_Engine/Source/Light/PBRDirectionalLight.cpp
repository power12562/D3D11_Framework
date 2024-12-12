#include "PBRDirectionalLight.h"

namespace DirectionalLight
{
    cb_PBRDirectionalLight DirectionalLights;
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
