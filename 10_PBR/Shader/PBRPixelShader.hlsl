#include "Shared.fxh"
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
    float Metalness;
    float Roughness;
    float padPBRMaterial[2];
};

cbuffer cb_bool : register(b4)
{
    bool useMetalness;
    bool useRoughness;
}

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
    
    float3x3 TBN = float3x3(input.Tangent, input.BiTangent, input.Normal);
    float3 N;
    if (0.f < length(normalSample))
        N = normalize(mul(normalSample, TBN));
    else
        N = normalize(mul(input.Normal, TBN));
    
    float3 L = -normalize(LightDir);
    float3 V = normalize(MainCamPos - input.World);
    float3 H = normalize(L + V);
    float HDotN = max(0.f, dot(H, N));
    
    float4 ambient = LightAmbient;
    float4 finalColor = ambient;
    
    float metalness = Metalness;
    float roughness = Roughness;
    
    if (useMetalness)
        metalness = metalnessTexture.Sample(defaultSampler, input.Tex).r;
    if (useRoughness)
        roughness = roughnessTexture.Sample(defaultSampler, input.Tex).r;
    
    float3 F = FresnelReflection(H, V, albedoSample.rgb, metalness);
    float D = NormalDistribution(max(0.01, roughness), N, H);
    float G = GeometricAttenuation(N, L, V, roughness);
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);
    
    float4 diffuse = float4(albedoSample.rgb * kd / PI, 1.f);
    float4 specular = float4((F * D * G) / max(0.00001, 4.0 * max(0.0, dot(N, L)) * max(0.0, dot(N, V))), 1.f);
    finalColor += (diffuse + specular) * ComputeDirectionalLight(N, L) * LightDiffuse;
    
    finalColor += emissiveSample;
    finalColor.a = opacitySample;
    finalColor.rgb = LinearToGammaSpace(finalColor.rgb);

    return finalColor;
}