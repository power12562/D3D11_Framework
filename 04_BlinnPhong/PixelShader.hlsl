#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 txColor = txDiffuse.Sample(samLinear, input.Tex);
    float4 diffuse = saturate(dot(input.Norm, (float3) -LightDir) * LightDiffuse) * txColor * MaterialDiffuse;
    
    float4 ambient = LightAmbient * MaterialAmbient;
    
    float3 View = normalize(CamPos.xyz - input.World);
    float3 HalfVector = normalize(-LightDir.xyz+View);
    float fHDotN = max(0.0f, dot(HalfVector, input.Norm));
    float4 specular = pow(fHDotN, MaterialSpecularPower) * MaterialSpecular * LightSpecular;
    
    return ambient + diffuse + specular;
}