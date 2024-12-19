#include "Shared.hlsli"
SamplerState samLinear : register(s0);
SamplerComparisonState ShadowMapSampler : register(s1);

Texture2D txDiffuse : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D emissiveMap : register(t3);
Texture2D opacityMap : register(t4);

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

cbuffer cb_Material : register(b4)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialSpecularPower;
    float3 MaterialSpecularPad;
}

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
    if (0.f < length(mapNormal))
        input.Normal = normalize(mul(mapNormal * 2.0f - 1.0f, WorldNormalTransform));
    
    float4 diffuse = saturate(dot(input.Normal, (float3) -Lights[0].LightDir) * Lights[0].LightColor) * MaterialDiffuse * txColor;
    
    //3x3 PCF ���ø�    
    float2 offsets[9] =
    {
        float2(-1, -1), float2(0, -1), float2(1, -1),
            float2(-1, 0), float2(0, 0), float2(1, 0),
            float2(-1, 1), float2(0, 1), float2(1, 1)
    };
    float texelSize = 1.0 / SHADOW_MAP_SIZE;
    float shadowFactor[MAX_LIGHT_COUNT] = { 1.0f, 1.0f, 1.0f, 1.0f };
    [unroll]
    for (int i = 0; i < LightsCount; i++)
    {
        //�׸��� Ȯ��
        float currentShadowDepth = input.PositionShadows[i].z;
        float2 uv = input.PositionShadows[i].xy;
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
    diffuse *= shadowFactor[0];
      
    float4 ambient = Lights[0].LightIntensity / 1000.f * MaterialAmbient;
    
    float3 View = normalize(MainCamPos.xyz - input.World);
    float3 HalfVector = normalize(-Lights[0].LightDir.xyz + View);
    float fHDotN = max(0.0f, dot(HalfVector, input.Normal * mapNormal));
 
    float4 specular;
    specular = pow(fHDotN, MaterialSpecularPower) * MaterialSpecular * mapSpecular;
 
    float4 final = diffuse + specular + mapEmissive + ambient;
    final.rgb = LinearToGammaSpace(final.rgb);
    final.a = opacity;
    //return float4((input.Normal + 1) / 2, 1.0f);
    return final;
}