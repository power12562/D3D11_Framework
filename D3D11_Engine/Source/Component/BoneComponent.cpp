#include "BoneComponent.h"

BoneComponent::BoneComponent()
{
}

BoneComponent::~BoneComponent()
{
}

Matrix BoneComponent::GetBoneMatrix()
{
	return Matrix::CreateScale(transform.scale) * 
		Matrix::CreateFromQuaternion(transform.rotation) *
		Matrix::CreateTranslation(transform.position);
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
