#pragma once
#include <Component\Base\Component.h>

struct cbuffer_Light
{
    Vector3 CamPos;
    float CamPosPad{};

    Vector4 LightDir{ 0, 0, 1, 0 };
    Vector4 LightDiffuse{ 1, 1, 1, 1 };
    Vector4 LightAmbient{ 0, 0, 0, 0 };
    Vector4 LightSpecular{ 1, 1, 1, 1 };

    Vector4 MaterialAmbient{ 1, 1, 1, 1 };
    Vector4 MaterialDiffuse{ 1, 1, 1, 1 };
    Vector4 MaterialSpecular{ 1, 1, 1, 1 };
    float MaterialSpecularPower{ 500 };
    Vector3 MaterialSpecularPad;
};

class LightManager : public Component
{
public:
    inline static cbuffer_Light cb_Light{};
public:
    LightManager();
    virtual ~LightManager() override;
public:
    virtual void Start() override; 
protected:
    virtual void FixedUpdate()override;
    virtual void Update() override;
    virtual void LateUpdate() override;
};