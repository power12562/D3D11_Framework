#include "Shared.fxh"
SamplerState defaultSampler : register(s0);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D emissiveTexture : register(t3);
Texture2D opacityTexture : register(t4);
Texture2D metalnessTexture : register(t5);
Texture2D roughnessTexture : register(t6);

cbuffer cb_Light : register(b2)
{
    float4 LightDir;
    float4 LightColor;
    float4 LightAmbient;
    float  LightIntensity; 
}

cbuffer cb_PBRMaterial : register(b3)
{
    float4 baseColor;
    float Metalness;
    float Roughness;
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

    float3 N; 
    if (Epsilon < length(normalSample))
    {
        float3x3 TBN = float3x3(input.Tangent, input.BiTangent, input.Normal);
        N = normalize(mul(normalSample * 2.0f - 1.0f, TBN));
    }
    else
        N = input.Normal;
    
    float3 L = normalize(-LightDir.xyz); // ���� ����
    float3 V = normalize(MainCamPos - input.World); // �� ����
    float3 H = normalize(L + V); // Half Vector

    float NoL = max(0.0, dot(N, L)); // N��L ���
    float NoV = max(0.0, dot(N, V)); // N��V ���
    float NoH = max(0.0, dot(N, H)); // N��H ���
    float HoV = max(0.0, dot(L, H)); // H��V ���
    
    // ���� Ư��
    float metalness = Metalness;
    float roughness = Roughness;

    if (Epsilon < metalnessSample)
        metalness = metalnessSample;
    if (Epsilon < roughnessSample)
        roughness = roughnessSample;
    
    float3 albedo = albedoSample.rgb * baseColor.rgb;

    // ������ �ݻ�
    float3 F = FresnelReflection(HoV, albedo, metalness);

    // ���� ���� �Լ�
    float D = NormalDistribution(roughness, NoH);

    // ��⼺ ����
    float G = GeometricAttenuation(NoV, NoL, roughness);

    // Specular �ݻ�
    float3 specular = SpecularBRDF(D, F, G, NoL, NoV);

    // Diffuse �ݻ�
    float3 diffuse = DiffuseBRDF(albedo, F, NoL, metalness);

    // ���� ���
    float3 radiance = LightColor.rgb * LightIntensity;
    float3 lighting = (diffuse + specular) * radiance * NoL;

    // ���� ����
    float3 ambient = LightAmbient.rgb * albedo; // ȯ�汤
    float3 finalColor = ambient + lighting + emissiveSample.rgb;
    finalColor = LinearToGammaSpaceExact(finalColor);

    return float4(finalColor, opacitySample);
}