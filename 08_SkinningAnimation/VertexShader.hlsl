#include "Shared.fxh"

#ifdef VERTEX_SKINNING
cbuffer MatrixPallete : register(b2)
{
    matrix MatrixPalleteArray[128];
}
#endif

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;  
    Matrix matWorld;
#ifdef VERTEX_SKINNING
    matWorld = mul(input.BlendWeights.x, MatrixPalleteArray[input.BlendIndecses.x]);
    matWorld += mul(input.BlendWeights.y, MatrixPalleteArray[input.BlendIndecses.y]);
    matWorld += mul(input.BlendWeights.z, MatrixPalleteArray[input.BlendIndecses.z]);
    matWorld += mul(input.BlendWeights.w, MatrixPalleteArray[input.BlendIndecses.w]);
#else
    matWorld = World;
#endif       
    output.World = (float3) mul(input.Pos, matWorld);
    output.Pos = mul(input.Pos, WVP);
     
#ifdef VERTEX_SKINNING
    output.Normal = normalize(mul(input.Normal, (float3x3) matWorld));
    output.Tangent = normalize(mul(input.Tangent, (float3x3) matWorld));
#else
    output.Normal = normalize(mul(input.Normal, (float3x3) WorldInverseTranspose));
    output.Tangent = normalize(mul(input.Tangent, (float3x3) WorldInverseTranspose));
#endif  
    output.BiTangent = normalize(cross(output.Normal, output.Tangent));
    
    output.Tex = input.Tex;
   
    return output;
}