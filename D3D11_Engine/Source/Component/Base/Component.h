#pragma once
#include <memory>

class Transform;
class GameObject;
class Component
{
	friend class GameObject;
public:
	Component();
	virtual ~Component();
	/** �߰������� ����ȭ�� ������ �ʿ�� �������̵�*/
	virtual void Serialized(std::ofstream& ofs) {};
	/** �߰������� ����ȭ�� ������ �ʿ�� �������̵�*/
	virtual void Deserialized(std::ifstream& ifs) {};
private:
	void SetOwner(GameObject* gameObject);
	int index = -1;

private:
	GameObject* _gameObject = nullptr;

public:
	virtual void Start()		= 0; //�ʱ�ȭ �Լ��� ��������� ȣ�� ����.
protected:
	virtual void FixedUpdate()  = 0;
	virtual void Update()		= 0;
	virtual void LateUpdate()	= 0;

public:
	bool Enable = true;

public:
	GameObject& GetGameObject() const;
	_declspec (property(get = GetGameObject)) GameObject& gameObject;

	Transform& GetTransform() const;
	Transform& SetTransform(const Transform& tr);
	__declspec (property(get = GetTransform, put = SetTransform)) Transform& transform;

	template <typename T>
	T& AddComponent();

	template <typename T>
	T& GetComponent();

	template <typename T>
	T* IsComponent();

	int GetComponentIndex() const { return index; }
};

#include <GameObject\Base\GameObject.h>
template<typename T>
inline T& Component::AddComponent()
{
	return gameObject.AddComponent<T>();
}

template<typename T>
inline T& Component::GetComponent()
{
	return gameObject.GetComponent<T>();
}

template<typename T>
inline T* Component::IsComponent()
{
	return gameObject.IsComponent<T>();
}
