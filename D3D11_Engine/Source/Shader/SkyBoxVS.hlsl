#include "Shared.fxh"
#include "SkyBoxShared.hlsli"

PixelShaderInput main(VS_INPUT input)
{
    PixelShaderInput output = (PixelShaderInput)0;
    float4 pos = mul(input.Pos, World);
    output.TexCoords = pos.xyz;
    
    // ����Ŀ��� ��ġ�� ������� ī�޶� ȸ���� �ݿ���
    float3 pos3 = mul(pos.xyz, (float3x3) View);
    pos = mul(float4(pos3, 1.0f), Projection);
    output.PositionProj = pos;
    return output;
}