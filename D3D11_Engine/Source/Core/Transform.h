#pragma once
#include <directxtk/simplemath.h>
#include <vector>
using namespace DirectX::SimpleMath;

class GameObject;
class Transform
{
	friend GameObject;
	friend class SceneManager;
	GameObject* _gameObject = nullptr;
public:
	Transform();
	~Transform();

	Transform(const Transform& rhs);

	Transform& operator=(const Transform& rhs);
public:
	GameObject& GetGameObject() const;
	__declspec(property(get = GetGameObject)) GameObject& gameObject;

	const Vector3& SetPosition(const Vector3& value);
	const Vector3& GetPosition() const { return _position; }
	__declspec(property(get = GetPosition, put = SetPosition)) const Vector3& position;

	const Vector3& SetLocalPosition(const Vector3& value);
	const Vector3& GetLocalPosition() const { return _localPosition; }
	__declspec(property(get = GetLocalPosition, put = SetLocalPosition)) const Vector3& localPosition;
						   
	const Quaternion& SetRotation(const Quaternion& value);
	const Quaternion& SetRotation(const Vector3& value);
	const Quaternion& GetRotation() const { return _rotation; }
	__declspec(property(get = GetRotation, put = SetRotation)) const Quaternion& rotation;

	const Quaternion& SetLocalRotation(const Quaternion& value);
	const Quaternion& SetLocalRotation(const Vector3& value);
	const Quaternion& GetLocalRotation() const { return _localRotation; }
	__declspec(property(get = GetLocalRotation, put = SetLocalRotation)) const Quaternion& localRotation;

	const Vector3& SetScale(const Vector3& value);
	const Vector3& SetScale(float value);
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

	/*이번 프레임 WM 업데이트 여부*/
	inline bool IsUpdateWM() const { return rootParent ? rootParent->isUpdateWM : isUpdateWM; }

	/** isUpdateWM 플래그 초기화*/
	inline void ResetFlagUpdateWM() { rootParent ? rootParent->isUpdateWM = false : isUpdateWM = false; }

	/**Get world matrix*/
	const Matrix& GetWM() const { return _WM; }

	/**Get local matrix*/
	const Matrix& GetLM() const { return _LM; }
public:
	/**부모로 만들 Transform을 설정합니다.*/
	void SetParent(Transform& parent, bool worldPositionStays = true);
	/**부모로 만들 Transform을 설정합니다. nullptr 전달시 해제합니다.*/
	void SetParent(Transform* parent = nullptr, bool worldPositionStays = true);

	/**해당 인덱스의 자식을 반환합니다. 실패하면 nullptr을 반환합니다.*/
	Transform* GetChild(unsigned int index);

	/** 자식 트랜스폼의 개수*/
	unsigned int GetChildCount() { return (unsigned int)childList.size(); }

	/*부모 트랜스폼*/
	Transform* GetParent() { return parent; }
	__declspec(property(get = GetParent)) Transform* Parent;

	/*최상위 부모*/
	Transform* GetRootParent() const { return rootParent; }
	__declspec(property(get = GetRootParent)) Transform* RootParent;
public:
	/*Transform 정보로 메트릭스를 업데이트합니다.*/
	void UpdateTransform();

private:
	void UpdateChildTransform();

private:
	void ClearParent();
	void SetChildsRootParent(Transform* _rootParent);

private:
	bool IsDescendantOf(Transform* potentialAncestor) const;

private:
	Transform* parent{ nullptr }; 
	Transform* rootParent{ nullptr };
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

private:
	bool isUpdateWM = false;
};
