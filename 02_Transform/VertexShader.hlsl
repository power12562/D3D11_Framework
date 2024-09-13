#include <Shared.fxh>

PS_INPUT main(float4 Pos : POSITION, float4 Color : COLOR)
{
    PS_INPUT output;
    output.Pos = mul(Pos, WVP);
    output.Color = Color;
    return output;
}