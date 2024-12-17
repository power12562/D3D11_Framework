#include "Shared.fxh"
SamplerState defaultSampler : register(s0);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D emissiveTexture : register(t3);
Texture2D opacityTexture : register(t4);
Texture2D metalnessTexture : register(t5);
Texture2D roughnessTexture : register(t6);

cbuffer cb_Light : register(b3)
{
    struct
    {
        float4 LightColor;
        float3 LightDir;
        float LightIntensity;
    }
    Lights[4];
    int LightsCount;
}

cbuffer cb_PBRMaterial : register(b4)
{
    float4 baseColor;
    float Metalness;
    float Roughness;
    
    bool UseMetalnessMap;
    bool UseRoughnessMap;
    bool UseAmbientOcculusion;
    bool UseRMACMap;
};

cbuffer cb_bool : register(b5)
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
    albedoSample.rgb = GammaToLinearSpaceExact(albedoSample.rgb);
    float3 normalSample = normalTexture.Sample(defaultSampler, input.Tex).rgb;
    float4 emissiveSample = emissiveTexture.Sample(defaultSampler, input.Tex);
    emissiveSample.rgb = GammaToLinearSpaceExact(emissiveSample.rgb);
    float opacitySample = opacityTexture.Sample(defaultSampler, input.Tex).a;

    // ���� Ư��
    float metalness = Metalness;
    float roughness = Roughness;

    if (useMetalness)
        metalness = metalnessTexture.Sample(defaultSampler, input.Tex).r;
    if (useRoughness)
        roughness = roughnessTexture.Sample(defaultSampler, input.Tex).r;

    float3 albedo = albedoSample.rgb * baseColor.rgb;
    
    float3 N; 
    if (Epsilon < length(normalSample))
    {
        float3x3 TBN = float3x3(input.Tangent, input.BiTangent, input.Normal);
        N = normalize(mul(normalSample * 2.0f - 1.0f, TBN));
    }
    else
        N = input.Normal;
    
    float3 V = normalize(MainCamPos - input.World); // �� ����
    float NoV = max(0.0, dot(N, V)); // N��V ���
    
    float3 finalColor;
    for (int i = 0; i < LightsCount; i++)
    {
        float3 L = normalize(-Lights[i].LightDir.xyz); // ���� ����
        float3 H = normalize(L + V); // Half Vector

        float NoL = max(0.0, dot(N, L)); // N��L ���
        float NoH = max(0.0, dot(N, H)); // N��H ���
        float HoV = max(0.0, dot(L, H)); // H��V ���
   
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
        float3 radiance = Lights[i].LightColor.rgb * Lights[i].LightIntensity;
        float3 lighting = (diffuse + specular) * radiance * NoL;

    // ���� ����
      
        finalColor += lighting;
    }  
    float3 ambient = 0.011 * albedo; // ȯ�汤
    finalColor += ambient + emissiveSample.rgb;
    finalColor = LinearToGammaSpaceExact(finalColor);

    return float4(finalColor, opacitySample);
}