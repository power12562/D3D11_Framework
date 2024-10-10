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
	std::string directory;

private:
	DRAW_INDEX_DATA drawData;

private:
	ID3D11ShaderResourceView* m_pTextureRV = nullptr;
	ID3D11ShaderResourceView* m_pNormalMap = nullptr;
	ID3D11ShaderResourceView* m_pSpecularMap = nullptr;

	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.
};