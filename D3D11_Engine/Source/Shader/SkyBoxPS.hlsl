#include "Shared.fxh"
#include "SkyBoxShared.hlsli"
SamplerState samLinear : register(s0);
TextureCube envTexture : register(t0);

float4 main(PixelShaderInput input) : SV_Target
{
    return envTexture.Sample(samLinear, input.TexCoords);
}
