#include "Shared.fxh"
SamplerState samLinear : register(s0);

cbuffer cb_Material : register(b2)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialSpecularPower;
    float3 MaterialSpecularPad;
}

cbuffer cbuffer_Light : register(b3)
{
    float4 LightDir;
    float4 LightDiffuse;
    float4 LightAmbient;
    float4 LightSpecular;
}

cbuffer cbuffer_bool : register(b4)
{
    bool UseNormalMap;
    bool UseSpecularMap;
    bool UseEmissive;
    bool UseOpacity;
}

cbuffer cb_localBool : register(b5)
{
    bool loaclNormal;
    bool loaclSpecular;
    bool loaclEmissive;
    bool loaclOpacity;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{   
    float4 opacity = { 0, 0, 0, 1 };
    
    float3x3 WorldNormalTransform = float3x3(input.Tangent, input.BiTangent, input.Normal);
    float4 diffuse = saturate(dot(input.Normal, (float3) -LightDir) * LightDiffuse) * MaterialDiffuse;

    float4 ambient = LightAmbient * MaterialAmbient;
    
    float3 View = normalize(MainCamPos.xyz - input.World);
    float3 HalfVector = normalize(-LightDir.xyz+View);
    float fHDotN = max(0.0f, dot(HalfVector, input.Normal));
 
    float4 specular;
    specular = pow(fHDotN, MaterialSpecularPower) * MaterialSpecular * LightSpecular;
 
    float4 final = ambient + diffuse + specular;
    final.a = opacity.a;
    
    //return MaterialDiffuse;
    return final;
}