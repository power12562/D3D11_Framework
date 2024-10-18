#pragma once
#include <Component\Base\Component.h>
#include <Material\SimpleMaterial.h>

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

namespace Global_Cbuffer
{
    extern cbuffer_bool cb_bool;
    extern cb_Material cb_material;
};


