#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 objColor = 0;
    //do NdotL lighting for 2 lights

    objColor.r = 1;
    objColor.g = 0;
    objColor.b = 0;
    objColor.a = 1;
    float4 finalColor = (saturate(dot(input.Norm, (float3) -vLightDir) * vLightColor), 1) * objColor;
    return finalColor;
}