#pragma once
#include <Utility/D3D11Utility.h>

#pragma warning(disable : 4305)
struct alignas(16) cb_PBRDirectionalLight
{
    DirectX::SimpleMath::Vector4 LightDir{ 0, -1, 1, 0 };
    DirectX::SimpleMath::Vector4 LightColor{ 1, 1, 1, 1 };
    DirectX::SimpleMath::Vector4 LightAmbient{ 0.01f, 0.01f, 0.01f, 0.f };
    float LightIntensity = 5.f;
};
#pragma warning(default : 4305)