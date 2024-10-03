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
	Vector3& SetPosition(const Vector3& value);
	const Vector3& GetPosition() const { return _position; }
	__declspec(property(get = GetPosition, put = SetPosition)) const Vector3& position;

	Vector3& SetLocalPosition(const Vector3& value);
	const Vector3& GetLocalPosition() const { return _localPosition; }
	__declspec(property(get = GetLocalPosition, put = SetLocalPosition)) const Vector3& localPosition;
						   
	Vector3& SetRotation(const Vector3& value);
	const Vector3& GetRotation() const { return _rotation; }
	__declspec(property(get = GetRotation, put = SetRotation)) const Vector3& rotation;

	Vector3& SetLocalRotation(const Vector3& value);
	const Vector3& GetLocalRotation() const { return _localRotation; }
	__declspec(property(get = GetLocalRotation, put = SetLocalRotation)) const Vector3& localRotation;

	Vector3& SetScale(const Vector3& value);
	const Vector3& GetScale() const { return _scale; }
	__declspec(property(get = GetScale, put = SetScale)) const Vector3& scale;

	Vector3& SetLocalScale(const Vector3& value);
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
	/**부모로 만들 Transform을 설정합니다.*/
	void SetParent(Transform& parent, bool worldPositionStays = true);
	/**부모로 만들 Transform을 설정합니다. nullptr 전달시 해제합니다.*/
	void SetParent(Transform* parent = nullptr, bool worldPositionStays = true);

	/**해당 인덱스의 자식을 반환합니다. 실패하면 nullptr을 반환합니다.*/
	Transform* GetChild(unsigned int index);

public:
	/*Transform 정보로 메트릭스를 업데이트합니다.*/
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

	Vector3 _rotation{};
	Vector3 _localRotation{};

	Vector3 _scale{1,1,1};
	Vector3 _localScale{1,1,1};

private:
	Matrix _WM{};
	Matrix _LM{};
	Matrix _rotationMatrix{};

};

