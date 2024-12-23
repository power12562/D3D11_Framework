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
    Matrix IVM;
    Matrix IPM;
};

cbuffer cb_ShadowMap : register(b2)
{
    Matrix ShadowProjection;
    Matrix ShadowView;
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







