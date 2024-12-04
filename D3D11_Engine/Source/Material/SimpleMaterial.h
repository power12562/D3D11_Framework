#pragma once
#include <Framework\D3DConstBuffer.h>
#include <directxtk/simplemath.h>
#include <array>
#include <memory>

#pragma warning( disable : 4305 )
struct cb_Material
{
	DirectX::SimpleMath::Color MaterialAmbient{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Color MaterialDiffuse{ 1, 1, 1, 1 };
	DirectX::SimpleMath::Color MaterialSpecular{ 1, 1, 1, 1 };
	float MaterialSpecularPower{ 300 };
	DirectX::SimpleMath::Vector3 MaterialSpecularPad;
};
#pragma warning( default : 4305 )

class SimpleMaterial
{
	friend class D3DRenderer;
public:
	enum TEXTURE_INDEX
	{
		Diffuse,
		Normal,
		Specular,
		Emissive,
		Opacity,
		Null
	};

public:
	SimpleMaterial() = default;
	~SimpleMaterial() = default;
public:
	cb_Material cb_material;
};		  

using sptrSimpleMaterial = std::shared_ptr<SimpleMaterial>;