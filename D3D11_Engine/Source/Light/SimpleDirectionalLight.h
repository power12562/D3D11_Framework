#pragma once
#include <Utility/D3D11Utility.h>

#pragma warning(disable : 4305)
struct alignas(16) cb_DirectionalLight
{
    DirectX::SimpleMath::Vector4 LightDir{ 0, -1, 1, 0 };
    DirectX::SimpleMath::Vector4 LightColor{ 1, 1, 1, 1 };
    DirectX::SimpleMath::Vector4 LightAmbient{ 0.02f, 0.02f, 0.02f, 0.f };
    DirectX::SimpleMath::Vector4 LightSpecular{ 1, 1, 1, 1 };
};
#pragma warning(default : 4305)

namespace SimpleDirectionalLight 
{
    extern cb_DirectionalLight cb_light;
};


