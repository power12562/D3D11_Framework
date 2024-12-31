#include "Shared.fxh"
SamplerState samLinear : register(s0);
Texture2D txDiffuse : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D emissiveMap : register(t3);
Texture2D opacityMap : register(t4);

cbuffer cb_Material : register(b3)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialSpecularPower;
    float3 MaterialSpecularPad;
}

cbuffer cbuffer_Light : register(b4)
{
    float4 LightDiffuse;
    float4 LightAmbient;
    float4 LightSpecular;
    float3 LightDir;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 txColor = txDiffuse.Sample(samLinear, input.Tex);
    txColor.rgb = GammaToLinearSpace(txColor.rgb);
    
    float3 mapNormal = normalMap.Sample(samLinear, input.Tex).rgb;
    float4 mapSpecular = specularMap.Sample(samLinear, input.Tex);
    float4 mapEmissive = emissiveMap.Sample(samLinear, input.Tex);
    mapEmissive.rgb = GammaToLinearSpace(mapEmissive.rgb);
    
    float opacity = opacityMap.Sample(samLinear, input.Tex).a;
    
    float3x3 WorldNormalTransform = float3x3(input.Tangent, input.BiTangent, input.Normal);
    
    if (0.00001f < length(mapNormal))
        input.Normal = normalize(mul(mapNormal * 2.0f - 1.0f, WorldNormalTransform));
    
    float4 diffuse = saturate(dot(input.Normal, (float3) -LightDir) * LightDiffuse) * MaterialDiffuse * txColor;

    float4 ambient = LightAmbient * MaterialAmbient;
    
    float3 View = normalize(MainCamPos.xyz - input.World);
    float3 HalfVector = normalize(-LightDir.xyz + View);
    float fHDotN = max(0.0f, dot(HalfVector, input.Normal * mapNormal));
 
    float4 specular;
    specular = pow(fHDotN, MaterialSpecularPower) * MaterialSpecular * LightSpecular * mapSpecular;
 
    float4 final = diffuse + specular + mapEmissive + ambient;
    final.rgb = LinearToGammaSpace(final.rgb);
    final.a = opacity;
    //return float4((input.Normal + 1) / 2, 1.0f);
    return final;
}