#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 objColor = float4(1, 0, 0, 1);
   
    float4 finalColor = saturate(dot(input.Norm, (float3)-vLightDir) * vLightColor) * objColor;
    return finalColor;
}