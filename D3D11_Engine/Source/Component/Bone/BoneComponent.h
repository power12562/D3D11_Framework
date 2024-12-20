#pragma once
#include <Component/Base/Component.h>

class BoneComponent : public Component
{
public:
	BoneComponent();
	virtual ~BoneComponent() override;

public:
	/*Get world matrix*/
	const Matrix& GetBoneMatrix();
public:
	int myIndex = -1;

public:
	virtual void Start()		override;
protected:						
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
};