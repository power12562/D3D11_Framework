#include "PBRShared.fxh"

static const float PI = 3.141592;
static const float Epsilon = 0.00001;

static const uint NumLights = 3;

static const float3 Fdielectric = 0.04;

SamplerState defaultSampler : register(s0);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D emissiveTexture : register(t3);
Texture2D opacityTexture : register(t4);
Texture2D metalnessTexture : register(t5);
Texture2D roughnessTexture : register(t6);

cbuffer cbuffer_Light : register(b2)
{
    float4 LightDir;
    float4 LightDiffuse;
    float4 LightAmbient;
    float4 LightSpecular;
}

cbuffer cb_PBRMaterial : register(b3)
{
    float4 baseColor;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{   
    float4 albedoSample = albedoTexture.Sample(defaultSampler, input.Tex);
    albedoSample.rgb = GammaToLinearSpace(albedoSample.rgb);
    
    float3 normalSample = normalTexture.Sample(defaultSampler, input.Tex).rgb * 2.0f - 1.0f;
    
    float4 specularSample = specularTexture.Sample(defaultSampler, input.Tex);
    
    float4 emissiveSample = emissiveTexture.Sample(defaultSampler, input.Tex);
    emissiveSample.rgb = GammaToLinearSpace(emissiveSample.rgb);
   
    float opacitySample = opacityTexture.Sample(defaultSampler, input.Tex).a;
    
    float3x3 WorldNormalTransform = float3x3(input.Tangent, input.BiTangent, input.Normal);
    input.Normal = normalize(mul(normalSample, WorldNormalTransform));
    
    float4 final = albedoSample * baseColor;
    final.rgb = LinearToGammaSpace(final.rgb);
    final.a = opacitySample;
    return final;
}