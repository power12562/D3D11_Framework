#pragma once
#include <Component\Base\Component.h>
#include <Framework\D3DConstBuffer.h>

struct cbuffer_Light
{
    Vector3 CamPos;
    float CamPosPad{};

    Vector4 LightDir{ 0, -1, 1, 0 };
    Vector4 LightDiffuse{ 1, 1, 1, 1 };
    Vector4 LightAmbient{ 0.01, 0.01, 0.01, 0.01 };
    Vector4 LightSpecular{ 1, 1, 1, 1 };

    Vector4 MaterialAmbient{ 1, 1, 1, 1 };
    Vector4 MaterialDiffuse{ 1, 1, 1, 1 };
    Vector4 MaterialSpecular{ 1, 1, 1, 1 };
    float MaterialSpecularPower{ 300 };
    Vector3 MaterialSpecularPad;
};

struct cbuffer_bool
{
    bool UseNormalMap = true;
    bool pad1[3]{};

    bool UseSpecularMap = true;
    bool pad2[3]{};

    bool UseEmissiveMap = true;
    bool pad3[3]{};

    bool UseOpacityMap = true;
    bool pad4[3]{};
};

struct cb_localBool
{
    bool loaclNormalMap = true;
    bool pad1[3]{};

    bool loaclSpecularMap = true;
    bool pad2[3]{};

    bool loaclEmissiveMap = true;
    bool pad3[3]{};

    bool loaclOpacityMap = true;
    bool pad4[3]{};
};

class SimpleDirectionalLight : public Component
{
public:
    inline static D3DConstBuffer cbuffer{};

public:
    inline static cbuffer_Light cb_Light{};
    inline static cbuffer_bool cb_bool{};
    inline static cb_localBool cb_localbool{};
public:
    SimpleDirectionalLight();
    virtual ~SimpleDirectionalLight() override;
public:
    virtual void Start() override; 
protected:
    virtual void FixedUpdate()override;
    virtual void Update() override;
    virtual void LateUpdate() override;
};