#pragma once
#include <Component/Base/Component.h>
#include <Framework/Transform.h>

class Camera : public Component
{
public:
	static Camera* GetMainCamera() { return mainCam; }
	static void SetMainCamera(Camera* pCamear) { mainCam = pCamear; }

private:
	inline static Camera* mainCam{};

public:
	float FOV = 90;
	float Near = 0.01f;
	float Far = 100.f;

public:
	Camera();
	virtual ~Camera() override;

public:
	const Matrix& GetVM() const;
	const Matrix& GetPM() const;

public:
	void SetMainCamera() { mainCam = this; }

public:
	virtual void Start() override;
protected:
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	Matrix view;
	Matrix projection;
};