#include <Shared.fxh>

cbuffer TestCbuffer : register(b2)
{
    float4 color;
}


float4 main(PS_INPUT input) : SV_TARGET
{
    return input.Color;
}