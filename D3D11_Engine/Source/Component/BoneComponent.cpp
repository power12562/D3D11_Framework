#include "BoneComponent.h"

BoneComponent::BoneComponent()
{
}

BoneComponent::~BoneComponent()
{
}

Matrix BoneComponent::GetBoneMatrix()
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
