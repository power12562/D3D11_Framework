#pragma once
#include <directxtk/simplemath.h>
#include <memory>

#pragma warning( disable : 4305 )
struct cb_PBRMaterial
{
    DirectX::SimpleMath::Color baseColor{1,1,1,1};
    float Metalness{};
    float Roughness{0.4f};
    float pad[2]{};
};
#pragma warning( default : 4305 )