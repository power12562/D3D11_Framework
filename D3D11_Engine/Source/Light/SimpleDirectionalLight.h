#pragma once
#include <Component\Base\Component.h>
#include <Material\SimpleMaterial.h>

#pragma warning(disable : 4305)
struct cbuffer_Light
{
    Vector4 LightDir{ 0, -1, 1, 0 };
    Vector4 LightDiffuse{ 1, 1, 1, 1 };
    Vector4 LightAmbient{ 0.01, 0.01, 0.01, 0.01 };
    Vector4 LightSpecular{ 1, 1, 1, 1 };
};
#pragma warning(default : 4305)

namespace SimpleDirectionalLight 
{
    extern cbuffer_Light cb_Light;
};


