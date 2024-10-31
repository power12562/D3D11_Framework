#include "Transform.h"
#include <Math/Mathf.h>
#include <GameObject/Base/GameObject.h>	
				
Transform::Transform()
{
	_WM = DirectX::XMMatrixIdentity();
	_LM = DirectX::XMMatrixIdentity();
}

Transform::~Transform()
{			
	if (!childList.empty())
	{
		for (auto child : childList)
		{
			child->parent = nullptr;
		}
	}
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

GameObject& Transform::GetGameObject()
{
	return *_gameObject;
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
			rootParent = rootParent->parent;
		}
		rootParent->UpdateTransform();

		// 부모의 역메트릭스을 자식의 메트릭스에 적용하여 로컬 회전을 계산
		Matrix localRotationMatrix = _WM * parent->_WM.Invert();
		Vector3 scale, translation;
		Quaternion quaternion;
		localRotationMatrix.Decompose(scale, quaternion, translation); 	
		_localRotation = quaternion;
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
			rootParent = rootParent->parent;
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
	Vector3 right = Vector3{ _WM._11, _WM._21, _WM._31 };
	right.Normalize();
	return right;
}

Vector3 Transform::GetUp()
{
	Vector3 up = Vector3{_WM._12, _WM._22, _WM._32};
	up.Normalize();
	return up;
}

Vector3 Transform::GetFront()
{
	Vector3 front = Vector3{ _WM._13, _WM._23, _WM._33 };
	front.Normalize();
	return front;
}

void Transform::SetParent(Transform& parent, bool worldPositionStays)
{				
	if (parent.rootParent == this || parent.parent == this || &parent == this->parent)
	{
		__debugbreak(); //자식을 부모로 설정 불가.
		return;
	}
		
	ClearParent();
	this->parent = &parent;	   

	if (parent.rootParent)
		this->rootParent = parent.rootParent;
	else
		this->rootParent = this->parent;

	SetChildsRootParent(rootParent);
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
		this->rootParent = nullptr;
		SetChildsRootParent(this);	
	}
}

void Transform::SetChildsRootParent(Transform* _rootParent)
{
	if (!childList.empty())
	{
		for (auto child : childList)
		{
			child->rootParent = _rootParent;
			child->SetChildsRootParent(_rootParent);
		}
	}
}
