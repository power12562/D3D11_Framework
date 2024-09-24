Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    Matrix World;
    Matrix View;
    Matrix WV;
    Matrix WVP;
   
    float4 CamPos;
    
    float4 LightDir;
    float4 LightDiffuse;
    float4 LightAmbient;
    float4 LightSpecular;

    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialSpecularPower;
    float3 MaterialSpecularPad;
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 World : POSITION;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
};







