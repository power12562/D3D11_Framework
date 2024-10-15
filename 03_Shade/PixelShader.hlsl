#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 objColor = txDiffuse.Sample(samLinear, input.Tex);
   
    float4 finalColor = saturate(dot(input.Norm, (float3) -vLightDir) * vLightColor) * objColor;
    finalColor.a = 1;
    return finalColor;
}