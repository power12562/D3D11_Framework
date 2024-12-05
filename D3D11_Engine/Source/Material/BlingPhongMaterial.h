#pragma once
#include <directxtk/simplemath.h>
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

using sptrBlingPhongMaterial = std::shared_ptr<cb_BlingPhongMaterial>;