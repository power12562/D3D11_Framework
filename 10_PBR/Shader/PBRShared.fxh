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