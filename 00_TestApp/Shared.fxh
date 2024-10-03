//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer cbuffer_Transform : register(b0)
{
    Matrix World;
    Matrix View;
    Matrix Projection;
    Matrix WVP;
}

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};