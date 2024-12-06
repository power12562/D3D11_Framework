#include "Shared.fxh"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 txColor = txDiffuse.Sample(samLinear, input.Tex);
    txColor.rgb = GammaToLinearSpace(txColor.rgb);
    
    float3 mapNormal = normalMap.Sample(samLinear, input.Tex).rgb * 2.0f - 1.0f;
    float4 mapSpecular = specularMap.Sample(samLinear, input.Tex);
    
    float3x3 WorldNormalTransform = float3x3(input.Tangent, input.BiTangent, input.Normal);
    if (UseNormalMap)
    {
        if (0.f < length(mapNormal))
            input.Normal = normalize(mul(mapNormal, WorldNormalTransform));
        else
            input.Normal = normalize(mul(input.Normal, WorldNormalTransform));
    }
    float4 diffuse = saturate(dot(input.Normal, (float3) -LightDir) * LightDiffuse) * txColor * MaterialDiffuse;
    
    float4 ambient = LightAmbient * MaterialAmbient;
    
    float3 View = normalize(CamPos.xyz - input.World);
    float3 HalfVector = normalize(-LightDir.xyz+View);
    float fHDotN = max(0.0f, dot(HalfVector, input.Normal));
 
    float4 specular;
    if(UseSpecularMap)
    {
        specular = pow(fHDotN, MaterialSpecularPower) * mapSpecular * MaterialSpecular *LightSpecular;
    }
    else
    {
        specular = pow(fHDotN, MaterialSpecularPower) * MaterialSpecular * LightSpecular;
    }
    
    //return float4(input.Normal, 1);
    //return specular;
    float4 final = ambient + diffuse + specular;
    final.rgb = LinearToGammaSpace(final.rgb);
    final.a = 1;
    return final;
}