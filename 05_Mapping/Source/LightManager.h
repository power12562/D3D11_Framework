#pragma once
#include <Component\Base\Component.h>
#include <Framework\D3DConstBuffer.h>

struct cb_DirectionalLight
{
    Vector3 CamPos;
    float CamPosPad{};

    Vector4 LightDir{ 0, 0, 1, 0 };
    Vector4 LightColor{ 1, 1, 1, 1 };
    Vector4 LightAmbient{ 0, 0, 0, 0 };
    Vector4 LightSpecular{ 1, 1, 1, 1 };

    Vector4 MaterialAmbient{ 1, 1, 1, 1 };
    Vector4 MaterialDiffuse{ 1, 1, 1, 1 };
    Vector4 MaterialSpecular{ 1, 1, 1, 1 };
    float MaterialSpecularPower{ 100 };
    Vector3 MaterialSpecularPad;
};

struct cbuffer_bool
{
    bool UseNormalMap = true;
    bool pad1[3]{};
    bool UseSpecularMap = true;
    bool pad2[11]{};
};

class LightManager : public Component
{
public:
    inline static D3DConstBuffer cbuffer{};

public:
    inline static cb_DirectionalLight cb_light{};
    inline static cbuffer_bool cb_bool{};
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