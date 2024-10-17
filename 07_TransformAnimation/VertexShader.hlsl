#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.World = (float3) mul(input.Pos, World);
    output.Pos = mul(input.Pos, WVP);
    
    output.Normal = normalize(mul(input.Normal, (float3x3) WorldInverseTranspose));
    output.Tangent = normalize(mul(input.Tangent, (float3x3) WorldInverseTranspose));
    output.BiTangent = normalize(cross(output.Normal, output.Tangent));
    
    output.Tex = input.Tex;
   
    return output;
}