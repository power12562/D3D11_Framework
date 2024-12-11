#include "Shared.fxh"
SamplerState defaultSampler : register(s0);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularMap : register(t2);
Texture2D emissiveTexture : register(t3);
Texture2D opacityTexture : register(t4);
Texture2D metalnessTexture : register(t5);
Texture2D roughnessTexture : register(t6);
Texture2D RMACTexture : register(t7);
Texture2D ambientOcculusionTexture : register(t8);

cbuffer cb_Light : register(b2)
{
    float4 LightColor;
    float4 LightAmbient;
    float3 LightDir;
    float  LightIntensity; 
}

cbuffer cb_PBRMaterial : register(b3)
{
    float4 baseColor;
    float Metalness;
    float Roughness;
    
    bool UseMetalnessMap;
    bool UseRoughnessMap;
    bool UseAmbientOcculusionMap;
    bool UseRMACMap;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{   
    float4 albedoSample = albedoTexture.Sample(defaultSampler, input.Tex);
    albedoSample.rgb = GammaToLinearSpaceExact(albedoSample.rgb);
    float3 normalSample = normalTexture.Sample(defaultSampler, input.Tex).rgb;
    float4 emissiveSample = emissiveTexture.Sample(defaultSampler, input.Tex);
    emissiveSample.rgb = GammaToLinearSpaceExact(emissiveSample.rgb);
    float opacitySample = opacityTexture.Sample(defaultSampler, input.Tex).a;
    float metalnessSample = metalnessTexture.Sample(defaultSampler, input.Tex).r;
    float roughnessSample = roughnessTexture.Sample(defaultSampler, input.Tex).r;
    float4 RMACSample = RMACTexture.Sample(defaultSampler, input.Tex);
    float ambientSample = ambientOcculusionTexture.Sample(defaultSampler, input.Tex).r;
    
    float3 N; 
    if (Epsilon < length(normalSample))
    {
        float3x3 TBN = float3x3(input.Tangent, input.BiTangent, input.Normal);
        N = normalize(mul(normalSample * 2.0f - 1.0f, TBN));
    }
    else
        N = input.Normal;
    
    float3 L = normalize(-LightDir.xyz); // 광원 방향
    float3 V = normalize(MainCamPos - input.World); // 뷰 방향
    float3 H = normalize(L + V); // Half Vector

    float NoL = max(0.0, dot(N, L)); // N·L 계산
    float NoV = max(0.0, dot(N, V)); // N·V 계산
    float NoH = max(0.0, dot(N, H)); // N·H 계산
    float HoV = max(0.0, dot(L, H)); // H·V 계산
    
    // 재질 특성
    float metalness = Metalness;
    float roughness = Roughness;
    float ambientOcculusion = 1;

    if (UseMetalnessMap)
        metalness = metalnessSample;
    else if (UseRMACMap)
        metalness = RMACSample.g;
    
    if (UseRoughnessMap)
        roughness = roughnessSample;
    else if (UseRMACMap)
        roughness = RMACSample.a;
    
    if (UseAmbientOcculusionMap)
        ambientOcculusion = ambientSample;
   else if(UseRMACMap)
        ambientOcculusion = RMACSample.b;
    
    float3 albedo = albedoSample.rgb * baseColor.rgb;

    // 프레널 반사
    float3 F = FresnelReflection(HoV, albedo, metalness);

    // 법선 분포 함수
    float D = NormalDistribution(roughness, NoH);

    // 폐쇄성 감쇠
    float G = GeometricAttenuation(NoV, NoL, roughness);

    // Specular 반사
    float3 specular = SpecularBRDF(D, F, G, NoL, NoV);

    // Diffuse 반사
    float3 diffuse = DiffuseBRDF(albedo, F, NoL, metalness);

    // 조명 계산
    float3 radiance = LightColor.rgb * LightIntensity;
    float3 directLighting = (diffuse + specular) * radiance * NoL;

    // 최종 색상
    float3 ambient = LightAmbient.rgb * albedo; // 환경광
    float3 finalColor = ambient + directLighting + emissiveSample.rgb;
    
    finalColor.rgb *= ambientOcculusion;
    
    finalColor = LinearToGammaSpaceExact(finalColor);
    return float4(finalColor, opacitySample);
}