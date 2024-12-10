#include "Shared.fxh"
#include "SkyBoxShared.hlsli"
SamplerState samLinear : register(s0);
TextureCube envTexture : register(t0);

float4 main(PixelShaderInput input) : SV_Target
{
    float4 final = envTexture.Sample(samLinear, input.TexCoords);
    final = float4(1, 0, 0, 1);
    return final;
}
