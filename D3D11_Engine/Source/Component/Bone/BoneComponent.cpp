#include "BoneComponent.h"
#include <Utility/D3D11Utility.h>

BoneComponent::BoneComponent()
{
}

BoneComponent::~BoneComponent()
{
}

const Matrix& BoneComponent::GetBoneMatrix()
{
	return transform.GetWM();
}

void BoneComponent::Start()
{
}

void BoneComponent::FixedUpdate()
{
}

void BoneComponent::Update()
{
}

void BoneComponent::LateUpdate()
{
}
