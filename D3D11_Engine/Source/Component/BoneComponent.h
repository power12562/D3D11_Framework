#pragma once
#include <Component/Base/Component.h>

struct MatrixPallete
{
	Matrix MatrixPalleteArray[128];
};

class BoneComponent : public Component
{
public:
	BoneComponent();
	virtual ~BoneComponent() override;

public:
	Matrix GetBoneMatrix();

public:
	std::shared_ptr<MatrixPallete> matrixPallete;
	int myIndex = -1;

public:
	virtual void Start()		override;
protected:						
	virtual void FixedUpdate()	override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
};