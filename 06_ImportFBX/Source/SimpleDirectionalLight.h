#pragma once
#include <Component\Base\Component.h>
#include <Material\SimpleMaterial.h>

#pragma warning(disable : 4305)
struct cbuffer_Light
{
    Vector4 LightDir{ 0, -1, 1, 0 };
    Vector4 LightDiffuse{ 1, 1, 1, 1 };
    Vector4 LightAmbient{ 0.01, 0.01, 0.01, 0.01 };
    Vector4 LightSpecular{ 1, 1, 1, 1 };
};
#pragma warning(default : 4305)

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

namespace SimpleDirectionalLight 
{
    extern cbuffer_Light cb_Light;
    extern cbuffer_bool cb_bool;
    extern cb_localBool cb_localbool;
    extern cb_Material cb_material;
};


