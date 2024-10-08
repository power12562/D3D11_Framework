#pragma once
#include <Component\Base\RenderComponent.h>

class AssimpComponent : public RenderComponent
{
public:
	AssimpComponent();
	virtual ~AssimpComponent() override;

public:
	virtual void Start()		override; //초기화 함수는 명시적으로 호출 가능.
protected:
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render()		override;

private:
	void LoadFBX(const char* path);

private:
	std::string directory;

};