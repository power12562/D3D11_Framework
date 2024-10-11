#pragma once
#include <Component\Base\RenderComponent.h>

class AssimpComponent : public RenderComponent
{
public:
	AssimpComponent();
	virtual ~AssimpComponent() override;

public:
	virtual void Start()		override; //�ʱ�ȭ �Լ��� ��������� ȣ�� ����.
protected:
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render()		override;

private:
	void LoadFBX(const char* path);

private:
	std::wstring directory;
	struct ID3D11RasterizerState* rasterierState = nullptr; //������ȭ ��Ģ
};