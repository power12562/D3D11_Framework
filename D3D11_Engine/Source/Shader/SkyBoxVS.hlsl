#include "Shared.hlsli"
#include "SkyBoxShared.hlsli"

PixelShaderInput main(float4 localPosition : POSITION)
{
    PixelShaderInput output = (PixelShaderInput)0;
    float4 pos = mul(localPosition, World);
    output.TexCoords = normalize(pos.xyz);
    
    // ����Ŀ��� ��ġ�� ������� ī�޶� ȸ���� �ݿ���
    float3 pos3 = mul(localPosition.xyz, (float3x3)View);
    pos = mul(float4(pos3, 1.f), Projection);
    output.PositionProj = pos;
    return output;
}