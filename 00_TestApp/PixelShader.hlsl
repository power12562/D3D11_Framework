#include <Shared.fxh>

cbuffer TestCbuffer : register(b2)
{
    float4 c;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    //return c;
    return input.Color;
}