#pragma once
#include <directxtk/simplemath.h>
#include <vector>
using namespace DirectX::SimpleMath;

class Transform
{
public:
	Transform();
	~Transform();
	
public:
	const Vector3& SetPosition(const Vector3& value);
	const Vector3& GetPosition() const { return _position; }
	__declspec(property(get = GetPosition, put = SetPosition)) const Vector3& position;

	const Vector3& SetLocalPosition(const Vector3& value);
	const Vector3& GetLocalPosition() const { return _localPosition; }
	__declspec(property(get = GetLocalPosition, put = SetLocalPosition)) const Vector3& localPosition;
						   
	const Quaternion& SetRotation(const Quaternion& value);
	const Quaternion& GetRotation() const { return _rotation; }
	__declspec(property(get = GetRotation, put = SetRotation)) const Quaternion& rotation;

	const Quaternion& SetLocalRotation(const Quaternion& value);
	const Quaternion& GetLocalRotation() const { return _localRotation; }
	__declspec(property(get = GetLocalRotation, put = SetLocalRotation)) const Quaternion& localRotation;

	const Vector3& SetScale(const Vector3& value);
	const Vector3& GetScale() const { return _scale; }
	__declspec(property(get = GetScale, put = SetScale)) const Vector3& scale;

	const Vector3& SetLocalScale(const Vector3& value);
	const Vector3& GetLocalScale() const { return _localScale; }
	__declspec(property(get = GetLocalScale, put = SetLocalScale)) const Vector3& localScale;

public:
	Vector3 GetRight();
	__declspec(property(get = GetRight)) Vector3 Right;

	Vector3 GetUp();
	__declspec(property(get = GetUp)) Vector3 Up;

	Vector3 GetFront();
	__declspec(property(get = GetFront)) Vector3 Front;

	const Matrix& GetWM() const { return _WM; }

public:
	/**�θ�� ���� Transform�� �����մϴ�.*/
	void SetParent(Transform& parent, bool worldPositionStays = true);
	/**�θ�� ���� Transform�� �����մϴ�. nullptr ���޽� �����մϴ�.*/
	void SetParent(Transform* parent = nullptr, bool worldPositionStays = true);

	/**�ش� �ε����� �ڽ��� ��ȯ�մϴ�. �����ϸ� nullptr�� ��ȯ�մϴ�.*/
	Transform* GetChild(unsigned int index);

public:
	/*Transform ������ ��Ʈ������ ������Ʈ�մϴ�.*/
	void UpdateTransform();
private:
	void UpdateChildTransform();

private:
	void ClearParent();

private:
	Transform* parent{ nullptr }; 
	std::vector<Transform*>	childList{};

private:
	Vector3 _position{};
	Vector3 _localPosition{};

	Quaternion _rotation{};
	Quaternion _localRotation{};

	Vector3 _scale{1,1,1};
	Vector3 _localScale{1,1,1};

private:
	Matrix _WM{};
	Matrix _LM{};
	Matrix _rotationMatrix{};

};
