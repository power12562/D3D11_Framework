#include "Shared.hlsli"
SamplerState defaultSampler : register(s0);
SamplerState BRDF_LUTSampler : register(s1);
SamplerComparisonState ShadowMapSampler : register(s2);

Texture2D AlbedoTexture : register(t0);     //Albedo rgb
Texture2D SpecularTexture : register(t1);   //Metallic.r + Specular.g + Roughness.b + AO.a
Texture2D NormalTexture : register(t2);     //Normal.rgb
Texture2D EmissiveTexture : register(t3);   //Emissive.rgb
Texture2D DepthTexture : register(t4);      //Depth.r

TextureCube Diffuse_IBL_Texture : register(t9);
TextureCube Specular_IBL_Texture : register(t10);
Texture2D BRDF_LUT : register(t11);

Texture2D ShadowMapTextures[4] : register(t124);

cbuffer cb_Light : register(b3)
{
    struct
    {
        float4 LightColor;
        float3 LightDir;
        float LightIntensity;
    }
    Lights[MAX_LIGHT_COUNT];
    int LightsCount;
}

struct PS_Deferred
{
    float4 svposition : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

inline float3 ReconstructWorldPosition(float2 uv, float depth)
{
    // Step 1: Reconstruct NDC
    float2 NDC = float2(uv.x * 2.0f - 1.0f, (1.0f - uv.y) * 2.0f - 1.0f);
    float4 clipSpacePos = float4(NDC, depth, 1.0f);

    // Step 2: Convert to View Space
    float4 viewSpacePos = mul(clipSpacePos, IPM);
    viewSpacePos = viewSpacePos / viewSpacePos.w;

    // Step 3: Convert to World Space
    float4 worldPos = mul(viewSpacePos, IVM);

    return worldPos.xyz;
}

inline float4 ComputePositionShadow(float3 world, matrix shadowView, matrix shadowProjection)
{
    float4 PositionShadows;
    PositionShadows = mul(float4(world, 1.0f), shadowView);
    PositionShadows = mul(PositionShadows, shadowProjection);
    return PositionShadows;
}

float4 main(PS_Deferred input) : SV_Target
{   
    float3 N = NormalTexture.Sample(defaultSampler, input.Tex).rgb * 2.0f - 1.f;
    N = normalize(N);
    clip(length(N) - Epsilon);
    
    float3 albedo = AlbedoTexture.Sample(defaultSampler, input.Tex).rgb;
    float3 emissive = EmissiveTexture.Sample(defaultSampler, input.Tex).rgb;
    float4 MSRAO = SpecularTexture.Sample(defaultSampler, input.Tex);
    
    float metalness = MSRAO.r;
    float specular  = MSRAO.g;
    float roughness = MSRAO.b;
    float ambientOcculusion = MSRAO.a;
    float Depth = DepthTexture.Sample(defaultSampler, input.Tex).r;
    float3 World = ReconstructWorldPosition(input.Tex, Depth);
    
    float4 PositionShadows[MAX_LIGHT_COUNT];
    [unroll]
    for (int i = 0; i < MAX_LIGHT_COUNT; i++)
    {
        PositionShadows[i] = ComputePositionShadow(World, ShadowViews[i], ShadowProjections[i]);
    }
    
    float3 F0 = lerp(Fdielectric, albedo, metalness); //F0 ���ϱ�
    F0 *= specular;
    
    float3 V = normalize(MainCamPos - World); // �� ����  
    float NoV = max(0.0, dot(N, V)); // N��V ���
       
    //3x3 PCF ���ø�    
    float2 offsets[9] =
    {
        float2(-1, -1), float2(0, -1), float2(1, -1),
        float2(-1, 0),  float2(0, 0),  float2(1, 0),
        float2(-1, 1),  float2(0, 1),  float2(1, 1)
    };
    float texelSize = 1.0 / SHADOW_MAP_SIZE;
    
    float shadowFactor[MAX_LIGHT_COUNT] = { 1.0f, 1.0f, 1.0f, 1.0f };
    [unroll]
    for (i = 0; i < LightsCount; i++)
    {
        //�׸��� Ȯ��
        float currentShadowDepth = PositionShadows[i].z;
        float2 uv = PositionShadows[i].xy;
        uv.y = -uv.y;
        uv = uv * 0.5f + 0.5f;
        
        if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
        {       
            [unroll]
            for (int j = 0; j < 9; j++)
            {
                float2 sampleUV = uv + offsets[j] * texelSize;
                shadowFactor[i] += ShadowMapTextures[i].SampleCmpLevelZero(ShadowMapSampler, sampleUV, currentShadowDepth - 0.001);
            }
            shadowFactor[i] = shadowFactor[i] / 9.0f;
        
            ////���Ͼ� �׸��� ���ø�
            //float sampleShadowDepth = ShadowMapTextures[i].Sample(defaultSampler, uv).r;
            //if (currentShadowDepth > sampleShadowDepth + 0.001)
            //{
            //    shadowFactor[i] = 0;
            //} 
        }
    }
    
    //���� ����
    float3 finalColor = 0;
    
    //directLighting
    for (i = 0; i < LightsCount; i++)
    {
        float3 L = normalize(-Lights[i].LightDir.xyz); // ���� ����
        float3 H = normalize(L + V); // Half Vector
        float NoL = max(0.0, dot(N, L)); // N��L ���
        float HoV = max(0.0, dot(L, H)); // H��V ���
        float NoH = max(0.0, dot(N, H)); // N��H ���
            
        // ������ �ݻ�
        float3 F = FresnelReflection(F0, HoV);

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
        float3 directLighting = (diffuse + specular) * radiance * NoL;
        finalColor += shadowFactor[i] * directLighting;
    }
    
    // ǥ���� �޴� �ݱ��� ���� ���⿡�� ���� ������ ���ø��Ѵ�. Lambertian BRDF�� �����Ͽ� ���ԵǾ� �ִ�.
    float3 irradiance = Diffuse_IBL_Texture.Sample(defaultSampler, N).rgb;
 
    // �� ������ Ư�� �� �� �����Ƿ� cosLo = dot(Normal,View)�� ����Ѵ�.
    float3 F = FresnelReflection(F0, NoV);
    
    // �ݼ��ϼ��� ǥ�� ����� �����ϸ� ��ݼ��ϼ��� ǥ�� ����� �״�� ǥ���ȴ�. 
    float3 kd = lerp(1.0 - F, 0.0, metalness);
    
    //IBL Diffuse
    float3 diffuseIBL = kd * albedo * irradiance;

    //Specular BRDF LookUpTable �̿��� ���Ѵ�.
    float2 specularBRDF = BRDF_LUT.Sample(BRDF_LUTSampler, float2(NoV, roughness)).rg;

    // ���� �ؽ�ó���� LOD Mipmap ���� ���� ��´�.
    uint SpecularTextureLevels, width, height;
    Specular_IBL_Texture.GetDimensions(0, width, height, SpecularTextureLevels);
 
    // Lr( View,Normal�� �ݻ纤��) �� ��ĥ�⸦ ����Ͽ� �ݻ� ���� ���ø��Ѵ�. 
    // ��ĥ�⿡ ���� ������ �ݻ� ���� ǥ���ϱ�����  LOD ���������� ����ȴ�. 
    float3 Lr = reflect(-V, N);
    float3 PrefilteredColor = Specular_IBL_Texture.SampleLevel(defaultSampler, Lr, roughness * SpecularTextureLevels).rgb;
    
    // IBL Specular 
    float3 specularIBL = SpecularIBL(F0, specularBRDF, PrefilteredColor);

    //���� ���
    float3 ambientLighting = (diffuseIBL + specularIBL) * ambientOcculusion;  
    finalColor += ambientLighting + emissive;
    
    finalColor = LinearToGammaSpaceExact(finalColor);
    return float4(finalColor, 1);
}