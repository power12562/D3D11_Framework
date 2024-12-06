static const float PI = 3.141592654;
static const float3 Fdielectric = 0.04;

inline float GammaToLinearSpaceExact(float value)
{
    if (value <= 0.04045f)
        return value / 12.92f;
    else if (value < 1.0f)
        return pow((value + 0.055f) / 1.055f, 2.4f);
    else
        return pow(value, 2.2f);
}

inline float3 GammaToLinearSpace(float3 rgb)
{
    return rgb * (rgb * (rgb * 0.305306011f + 0.682171111f) + 0.012522878f);

    //Precise version, useful for debugging.
    //return float3(GammaToLinearSpaceExact(rgb.r),
    //GammaToLinearSpaceExact(rgb.g),
    //GammaToLinearSpaceExact(rgb.b));
}

inline float LinearToGammaSpaceExact(float value)
{
    if (value <= 0.0f)
        return 0.0f;
    else if (value <= 0.0031308f)
        return 12.92f * value;
    else if (value < 1.0f)
        return 1.055f * pow(value, 0.4166667f) - 0.055f;
    else
        return pow(value, 0.45454545f);
}

inline float3 LinearToGammaSpace(float3 rgb)
{
    rgb = max(rgb, float3(0.f, 0.f, 0.f));
    return max(1.055f * pow(rgb, 0.416666667f) - 0.055f, 0.f);

    //Precise version, useful for debugging.
    //return float3(LinearToGammaSpaceExact(rgb.r),
    //LinearToGammaSpaceExact(rgb.g),
    //LinearToGammaSpaceExact(rgb.b));
}

inline float ComputeDirectionalLight(float3 normal, float3 lightDirection)
{
    return saturate(dot(normal, lightDirection));
}

inline float4 ComputePointLight()
{
    return float4(0.0, 0.0, 0.0, 0.0);
}

inline float NormalDistribution(float roughness, float3 N, float3 H)
{
    float alpha = roughness * roughness;
    float squareA = alpha * alpha;
    float NdotH = max(0.0, dot(N, H));
    float var = NdotH * NdotH * (squareA - 1.0) + 1.0;
    return squareA / (PI * var * var);
}

inline float3 FresnelReflection(float3 H, float3 V, float3 baseColor, float metalness)
{
    float3 F0 = lerp(Fdielectric, baseColor, metalness);
    return F0 + (1.0 - F0) * pow(1.0 - max(0.0, dot(H, V)), 5.0);
}

inline float GSchlickGGX(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

inline float GeometricAttenuation(float3 N, float3 L, float3 V, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // direct
    //float k = (roughness * roughness) * 0.5; // IBL
    return GSchlickGGX(max(0.0, dot(N, V)), k) * GSchlickGGX(max(0.0, dot(N, L)), k);
}

cbuffer cbuffer_Transform : register(b0)
{
    Matrix World;
    Matrix WorldInverseTranspose;
    Matrix WVP;
}

cbuffer cb_Camera : register(b1)
{
    float3 MainCamPos;
    Matrix View;
    Matrix Projection;
};


struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL0;
    float3 Tangent : NORMAL1;
    float2 Tex : TEXCOORD0;
    int4 BlendIndecses : BLENDINDICES;
    float4 BlendWeights : BLENDWEIGHT;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 World : POSITION;
    float3 Normal : NORMAL0;
    float3 Tangent : NORMAL1;
    float3 BiTangent : NORMAL2;
    float2 Tex : TEXCOORD0;
};