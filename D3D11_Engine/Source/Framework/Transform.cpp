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

	if (!rhs.parent)
	{
		_position = rhs._position;
		_rotation = rhs._rotation;
		_scale = rhs._scale;
	}
	else
	{
		_localPosition = rhs._localPosition;
		_localRotation = rhs._localRotation;
		_localScale = rhs._localScale;
	}
	return *this;
}

GameObject& Transform::GetGameObject()
{
	return *_gameObject;
}

const Vector3& Transform::SetPosition(const Vector3& value)
{
	if (Vector3::Distance(value, _position) > Mathf::Epsilon)
	{
		_position = value;
		if (parent)
			_localPosition = _position - parent->_position;
		transformChanged = true;
	}
	return _position;
}

const Vector3& Transform::SetLocalPosition(const Vector3& value)
{							  
	if (Vector3::Distance(value, _localPosition) > Mathf::Epsilon)
	{
		if (parent)
		{
			_localPosition = value;
			_position = parent->_position + _localPosition;
			rootParent->transformChanged = true;
		}
	}
	return _localPosition;
}

const Quaternion& Transform::SetRotation(const Quaternion& value)
{
	if (_localRotation.Dot(value) < 1.f - Mathf::Epsilon)
	{
		_rotation = value;
		if (parent)
		{
			Quaternion IProtation;
			parent->_rotation.Inverse(IProtation);
			_localRotation = _rotation * IProtation;
		}
		transformChanged = true;
	}
	return _rotation;
}

const Quaternion& Transform::SetRotation(const Vector3& value)
{
	Quaternion quater = Quaternion::CreateFromYawPitchRoll(value * Mathf::Deg2Rad);
	return SetRotation(quater);	
}

const Quaternion& Transform::SetLocalRotation(const Quaternion& value)
{
	if (_localRotation.Dot(value) < 1.f - Mathf::Epsilon)
	{
		if (parent)
		{
			_localRotation = value;
			_rotation = parent->_rotation * _localRotation;
			rootParent->transformChanged = true;
		}
	}
	return _localRotation;
}

const Quaternion& Transform::SetLocalRotation(const Vector3& value)
{
	Quaternion quater = Quaternion::CreateFromYawPitchRoll(value.y * Mathf::Deg2Rad, value.x * Mathf::Deg2Rad, value.z * Mathf::Deg2Rad);
	return SetLocalRotation(quater);
}

const Vector3& Transform::SetScale(const Vector3& value)
{
	if (Vector3::Distance(value, _scale) > Mathf::Epsilon)
	{
		_scale = value;
		if (parent)	
			_localScale = _scale / parent->scale;

		transformChanged = true;
	}
	return _scale;
}

const Vector3& Transform::SetLocalScale(const Vector3& value)
{
	if (Vector3::Distance(value, _scale) > Mathf::Epsilon)
	{
		if (parent)
		{
			_localScale = value;
			_scale = parent->scale * _localScale;
			transformChanged = true;
		}
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
	if (parent.rootParent == this || parent.parent == this || parent.IsDescendantOf(this))
	{
		Debug_printf("Error : Cannot set a child as a parent.\n");
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
	if (transformChanged && parent == nullptr)
	{
		_WM = DirectX::XMMatrixScalingFromVector(scale) *
			DirectX::XMMatrixRotationQuaternion(rotation) *
			DirectX::XMMatrixTranslationFromVector(position);

		UpdateChildTransform();
		transformChanged = false;
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

bool Transform::IsDescendantOf(Transform* potentialAncestor) const
{
	Transform* currentParent = parent;
	while (currentParent)
	{
		if (currentParent == potentialAncestor)
			return true;
		currentParent = currentParent->parent;
	}
	return false;
}