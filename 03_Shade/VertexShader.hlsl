#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(input.Pos, WVP);
    output.Norm = normalize(mul(input.Norm, (float3x3) World));
    output.Tex = input.Tex;
   
    return output;
}