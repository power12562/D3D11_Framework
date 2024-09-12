#include "Transform.h"
#include <Math/Mathf.h>
				
Transform::Transform()
{

}

Transform::~Transform()
{

}

Vector3& Transform::SetPosition(const Vector3& value)
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

Vector3& Transform::SetLocalPosition(const Vector3& value)
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

Vector3& Transform::SetRotation(const Vector3& value)
{
	if (parent)
	{
		_rotation = value;
		Matrix parentRotationMatrix = Matrix::CreateFromYawPitchRoll(
			parent->_rotation.y * Mathf::Deg2Rad, 
			parent->_rotation.x * Mathf::Deg2Rad,
			parent->_rotation.z * Mathf::Deg2Rad);
		Matrix childRotationMatrix = Matrix::CreateFromYawPitchRoll(
			_rotation.y * Mathf::Deg2Rad,
			_rotation.x * Mathf::Deg2Rad,
			_rotation.z * Mathf::Deg2Rad);

		// 부모의 역회전을 자식의 월드 회전에 적용하여 로컬 회전을 계산
		Matrix localRotationMatrix = childRotationMatrix * parentRotationMatrix.Invert();

		Vector3 scale, translation;
		Quaternion quaternion;
		localRotationMatrix.Decompose(scale, quaternion, translation); 	
		_localRotation = quaternion.ToEuler();
	}
	else
	{
		_rotation = value;
	}
	return _rotation;
}

Vector3& Transform::SetLocalRotation(const Vector3& value)
{
	if (parent)
	{
		_localRotation = value;
		Matrix parentRotationMatrix = Matrix::CreateFromYawPitchRoll(
			parent->_rotation.y * Mathf::Deg2Rad,
			parent->_rotation.x * Mathf::Deg2Rad,
			parent->_rotation.z * Mathf::Deg2Rad);
		Matrix childRotationMatrix = Matrix::CreateFromYawPitchRoll(
			_localRotation.y * Mathf::Deg2Rad,
			_localRotation.x * Mathf::Deg2Rad,
			_localRotation.z * Mathf::Deg2Rad);

		// 부모의 행렬로 자신의 월드 회전 행렬을 계산
		Matrix worldRotationMatrix = parentRotationMatrix * childRotationMatrix;

		Vector3 scale, translation;
		Quaternion quaternion;
		worldRotationMatrix.Decompose(scale, quaternion, translation);
		_rotation = quaternion.ToEuler();
	}
	else
	{
		
	}
	return _localRotation;
}

Vector3& Transform::SetScale(const Vector3& value)
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

Vector3& Transform::SetLocalScale(const Vector3& value)
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
	Matrix x = Matrix::CreateRotationX(_rotation.x * Mathf::Deg2Rad);
	Matrix y = Matrix::CreateRotationX(_rotation.y * Mathf::Deg2Rad);
	Matrix z = Matrix::CreateRotationX(_rotation.z * Mathf::Deg2Rad);

	_rotationMatrix = z * y * x;

	return Vector3{ _rotationMatrix._11, _rotationMatrix._21, _rotationMatrix._31 };
}

Vector3 Transform::GetUp()
{
	Matrix x = Matrix::CreateRotationX(_rotation.x * Mathf::Deg2Rad);
	Matrix y = Matrix::CreateRotationX(_rotation.y * Mathf::Deg2Rad);
	Matrix z = Matrix::CreateRotationX(_rotation.z * Mathf::Deg2Rad);

	_rotationMatrix = z * y * x;

	return Vector3{ _rotationMatrix._12, _rotationMatrix._22, _rotationMatrix._32 };
}

Vector3 Transform::GetFront()
{
	Matrix x = Matrix::CreateRotationX(_rotation.x * Mathf::Deg2Rad);
	Matrix y = Matrix::CreateRotationX(_rotation.y * Mathf::Deg2Rad);
	Matrix z = Matrix::CreateRotationX(_rotation.z * Mathf::Deg2Rad);

	_rotationMatrix = z * y * x;

	return Vector3{ _rotationMatrix._13, _rotationMatrix._23, _rotationMatrix._33 };
}

void Transform::SetParent(Transform& parent, bool worldPositionStays)
{				
	if (this->parent)
	{
		auto& pChildList = this->parent->childList;
		for (auto iter = pChildList.begin(); iter != pChildList.end(); iter++)
		{
			if (*iter == this)
			{
				pChildList.erase(iter);
				break;
			}
		}
		this->parent = nullptr;
	}
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
		auto& pChildList = this->parent->childList;
		for (auto iter = pChildList.begin(); iter != pChildList.end(); iter++)
		{
			if (*iter == this)
			{
				pChildList.erase(iter);
				break;
			}
		} 
		this->parent = nullptr;
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
