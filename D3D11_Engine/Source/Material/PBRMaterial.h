#pragma once
#include <directxtk/simplemath.h>
#include <memory>

struct alignas(16) cb_PBRMaterial
{
    DirectX::SimpleMath::Color baseColor{1,1,1,1};
    float Metalness{};
    float Roughness{};
};