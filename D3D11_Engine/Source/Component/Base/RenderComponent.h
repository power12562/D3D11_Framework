#pragma once
#include <Component\Base\Component.h>
#include <Framework\D3DRenderer.h>

class RenderComponent : public Component
{
	friend class GameObject;
public:
	RenderComponent();
	virtual ~RenderComponent() override;

public:
	virtual void Start() = 0; //�ʱ�ȭ �Լ��� ��������� ȣ�� ����.
protected:
	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;
};