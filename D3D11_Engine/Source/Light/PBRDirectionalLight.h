#pragma once
#include <Utility/D3D11Utility.h>

struct alignas(16) cb_PBRDirectionalLight
{
    DirectX::SimpleMath::Vector3 LightDir{ 0.f, -1.f, 1.f };
private: 
    float padding1;
public:
    DirectX::SimpleMath::Vector4 LightColor{ 1, 1, 1, 1 };
    DirectX::SimpleMath::Vector4 LightAmbient{ 0.01f, 0.01f, 0.01f, 0.f };
    float LightIntensity = 5.f;
};

namespace PBRDirectionalLight
{
    extern cb_PBRDirectionalLight cb_light;
};

