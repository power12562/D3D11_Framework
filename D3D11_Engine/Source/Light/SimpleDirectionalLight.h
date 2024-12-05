#pragma once
#include <Component\Base\Component.h>
#include <Material\BlingPhongMaterial.h>

#pragma warning(disable : 4305)
struct cb_Light
{
    Vector4 LightDir{ 0, -1, 1, 0 };
    Vector4 LightDiffuse{ 1, 1, 1, 1 };
    Vector4 LightAmbient{ 0.f, 0.f, 0.f, 0.f };
    Vector4 LightSpecular{ 1, 1, 1, 1 };
};
#pragma warning(default : 4305)

namespace SimpleDirectionalLight 
{
    extern cb_Light cb_light;
};


