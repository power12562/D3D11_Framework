cbuffer cbuffer_Transform : register(b0)
{
    Matrix World;
    Matrix WorldInverseTranspose;
    Matrix WVP;
}

cbuffer cbuffer_Camera : register(b1)
{
    Matrix View;
    Matrix Projection;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL0;
    float3 Tangent : NORMAL1;
    float2 Tex : TEXCOORD0;
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






