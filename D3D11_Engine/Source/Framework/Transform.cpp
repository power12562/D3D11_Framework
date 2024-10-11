#include "Transform.h"
#include <Math/Mathf.h>
				
Transform::Transform()
{
	_WM = DirectX::XMMatrixIdentity();
	_LM = DirectX::XMMatrixIdentity();
}

Transform::~Transform()
{								 
	ClearParent();
}

Transform::Transform(const Transform& rhs)
{
	_position = rhs._position;
	_rotation = rhs._rotation;
	_scale = rhs._scale;

	_localPosition = rhs._localPosition;
	_localRotation = rhs._localRotation;
	_localScale = rhs._localScale;
}

Transform& Transform::operator=(const Transform& rhs)
{
	if (this == &rhs)
		return *this;

	_position	= rhs._position;
	_rotation	= rhs._rotation;
	_scale		= rhs._scale;

	_localPosition	= rhs._localPosition;
	_localRotation	= rhs._localRotation;
	_localScale		= rhs._localScale;

	return *this;
}

const Vector3& Transform::SetPosition(const Vector3& value)
{
	if (parent)
	{
		_position = value;
		_localPosition = _position - parent->position;
	}
	else
	{
		_position = value;
	}
	return _position;
}

const Vector3& Transform::SetLocalPosition(const Vector3& value)
{
	if (parent)
	{
		_localPosition = value;
		_position = parent->position + value;
	}
	else
	{
		
	}
	return _localPosition;
}

const Quaternion& Transform::SetRotation(const Quaternion& value)
{
	if (parent)
	{
		_rotation = value;
		Transform* rootParent = parent;
		while (rootParent->parent != nullptr)
		{
			rootParent = parent->parent;
		}
		rootParent->UpdateTransform();

		// 부모의 역메트릭스을 자식의 메트릭스에 적용하여 로컬 회전을 계산
		Matrix localRotationMatrix = _WM * parent->_WM.Invert();
		Vector3 scale, translation;
		Quaternion quaternion;
		localRotationMatrix.Decompose(scale, quaternion, translation); 	
		_localRotation;
	}
	else
	{
		_rotation = value;
	}
	return _rotation;
}

const Quaternion& Transform::SetLocalRotation(const Quaternion& value)
{
	if (parent)
	{
		_localRotation = value;
		Transform* rootParent = parent;
		while (rootParent->parent != nullptr)
		{
			rootParent = parent->parent;
		}
		rootParent->UpdateTransform();

		Vector3 scale, translation;
		Quaternion quaternion;
		_WM.Decompose(scale, quaternion, translation);
		_rotation = quaternion;
	}
	else
	{
		
	}
	return _localRotation;
}

const Vector3& Transform::SetScale(const Vector3& value)
{
	if (parent)
	{
		_scale = value;
		_localScale = _scale / parent->scale;
	}
	else
	{
		_scale = value;
	}
	return _scale;
}

const Vector3& Transform::SetLocalScale(const Vector3& value)
{
	if (parent)
	{
		_localScale = value;
		_scale = parent->scale * _localScale;
	}
	else
	{
		
	}
	return _localScale;
}

Vector3 Transform::GetRight()
{
	_rotationMatrix = _rotationMatrix = Matrix::CreateFromQuaternion(_rotation);

	return Vector3{ _rotationMatrix._11, _rotationMatrix._21, _rotationMatrix._31 };
}

Vector3 Transform::GetUp()
{
	_rotationMatrix = Matrix::CreateFromQuaternion(_rotation);

	return Vector3{ _rotationMatrix._12, _rotationMatrix._22, _rotationMatrix._32 };
}

Vector3 Transform::GetFront()
{
	_rotationMatrix = Matrix::CreateFromQuaternion(_rotation);

	return Vector3{ _rotationMatrix._13, _rotationMatrix._23, _rotationMatrix._33 };
}

void Transform::SetParent(Transform& parent, bool worldPositionStays)
{				
	ClearParent();
	this->parent = &parent;
	parent.childList.push_back(this);
	if (worldPositionStays)
	{
		SetPosition(_position);
		SetRotation(_rotation);
		SetScale(_scale);
	}
	else
	{
		SetLocalPosition(_position);
		SetLocalRotation(_rotation);
		SetLocalScale(_scale);
	}
}

void Transform::SetParent(Transform* parent, bool worldPositionStays)
{
	if (parent)
	{
		SetParent(*parent, worldPositionStays);
	}
	else
	{
		ClearParent();
	}
}

Transform* Transform::GetChild(unsigned int index)
{
	if (index < childList.size())
	{
		return childList[index];
	}
	else
	{
		return nullptr;
	}
}

void Transform::UpdateTransform()
{
	if (parent == nullptr)
	{
		_WM = DirectX::XMMatrixScalingFromVector(scale) *
			DirectX::XMMatrixRotationQuaternion(rotation) *
			DirectX::XMMatrixTranslationFromVector(position);
		UpdateChildTransform();
	}
}

void Transform::UpdateChildTransform()
{
	if (!childList.empty())
	{
		for (auto child : childList)
		{		
			child->_LM = DirectX::XMMatrixScalingFromVector(child->localScale) *
				DirectX::XMMatrixRotationQuaternion(child->localRotation) *
				DirectX::XMMatrixTranslationFromVector(child->localPosition);
			child->_WM = child->_LM *_WM;
			child->UpdateChildTransform();
		}
	}
}

void Transform::ClearParent()
{
	if (parent)
	{
		auto& pChildList = this->parent->childList;
		if (!pChildList.empty())
		{
			for (auto iter = pChildList.begin(); iter != pChildList.end(); ++iter)
			{
				if (*iter == this)
				{
					pChildList.erase(iter);
					break;
				}
			}
		}
		this->parent = nullptr;
	}
}
