#include "Shared.fxh"
Texture2D txDiffuse : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D emissiveMap : register(t3);
Texture2D opacityMap : register(t4);

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
    float4 txColor = txDiffuse.Sample(samLinear, input.Tex);
    float3 mapNormal = normalMap.Sample(samLinear, input.Tex).rgb * 2.0f - 1.0f;
    float4 mapSpecular = specularMap.Sample(samLinear, input.Tex);
    float4 emissive = { 0, 0, 0, 0 };
    float4 mapOpacity = opacityMap.Sample(samLinear, input.Tex);
    
    if (UseEmissive && loaclEmissive)
    {
        emissive = emissiveMap.Sample(samLinear, input.Tex);
    }
      
    float4 opacity = { 0, 0, 0, 1 };
    if (UseOpacity && loaclOpacity)
    {
        opacity.a = mapOpacity.a;
    }
    
    float3x3 WorldNormalTransform = float3x3(input.Tangent, input.BiTangent, input.Normal);
    if (UseNormalMap && loaclNormal)
    {
        if (0.f < length(mapNormal))
            input.Normal = normalize(mul(mapNormal, WorldNormalTransform));
    }
    float4 diffuse = saturate(dot(input.Normal, (float3) -LightDir) * LightDiffuse) * txColor * MaterialDiffuse;

    float4 ambient = LightAmbient * MaterialAmbient;
    
    float3 View = normalize(MainCamPos.xyz - input.World);
    float3 HalfVector = normalize(-LightDir.xyz+View);
    float fHDotN = max(0.0f, dot(HalfVector, input.Normal));
 
    float4 specular;
    if (UseSpecularMap && loaclSpecular)
    {
        specular = pow(fHDotN, MaterialSpecularPower) * mapSpecular * MaterialSpecular *LightSpecular;
    }
    else
    {
        specular = pow(fHDotN, MaterialSpecularPower) * MaterialSpecular * LightSpecular;
    }
      
    //return float4((input.Normal + 1) / 2, 1.0f);
    //return specular;
    //return emissive;
    //return opacity;
    float4 final = ambient + diffuse + specular + emissive;
    final.a = opacity.a;
    return final;
}