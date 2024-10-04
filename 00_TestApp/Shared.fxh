//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};