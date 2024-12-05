#pragma once
#include <Framework\D3DConstBuffer.h>
#include <directxtk/simplemath.h>
#include <array>
#include <memory>

#pragma warning( disable : 4305 )
struct cb_BlingPhongMaterial
{
	DirectX::SimpleMath::Color MaterialAmbient{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Color MaterialDiffuse{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Color MaterialSpecular{ 1, 1, 1, 1 };
	float MaterialSpecularPower{ 300 };
	DirectX::SimpleMath::Vector3 MaterialSpecularPad;
};
#pragma warning( default : 4305 )

using sptrSimpleMaterial = std::shared_ptr<SimpleMaterial>;