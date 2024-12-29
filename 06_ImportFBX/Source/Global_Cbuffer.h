#pragma once
#include <Component\Base\Component.h>
#include <Material\BlingPhongMaterial.h>

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
    constexpr char cb_bool_key[] = "cb_bool_key";
    extern std::shared_ptr<cbuffer_bool> cb_bool;

    constexpr char cb_material_key[] = "cb_material_key";
    extern std::shared_ptr<cb_BlingPhongMaterial> cb_material;
};


