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
    if (value <= 0.04045F)
        return value / 12.92F;
    else if (value < 1.0F)
        return pow((value + 0.055F) / 1.055F, 2.4F);
    else
        return pow(value, 2.2F);
}

inline float4 GammaToLinearSpace(float4 rgba)
{
    return rgba * (rgba * (rgba * 0.305306011h + 0.682171111h) + 0.012522878h);
}

inline float3 GammaToLinearSpace(float3 rgb)
{
    return rgb * (rgb * (rgb * 0.305306011h + 0.682171111h) + 0.012522878h);
}

inline float LinearToGammaSpaceExact(float value)
{
    if (value <= 0.0F)
        return 0.0F;
    else if (value <= 0.0031308F)
        return 12.92F * value;
    else if (value < 1.0F)
        return 1.055F * pow(value, 0.4166667F) - 0.055F;
    else
        return pow(value, 0.45454545F);
}

inline float4 LinearToGammaSpace(float4 rgba)
{
    rgba = max(rgba, float4(0.f, 0.f, 0.f, 0.f));
    return max(1.055h * pow(rgba, 0.416666667h) - 0.055h, 0.f);
}

inline float3 LinearToGammaSpace(float3 rgb)
{
    rgb = max(rgb, half3(0.f, 0.f, 0.f));
    return max(1.055h * pow(rgb, 0.416666667h) - 0.055h, 0.f);
}
