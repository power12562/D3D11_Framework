#include "Shared.hlsli"
#include "SkyBoxShared.hlsli"

PixelShaderInput main(float4 localPosition : POSITION)
{
    PixelShaderInput output = (PixelShaderInput)0;
    output.TexCoords = normalize(localPosition.xyz);
    
    // ����Ŀ��� ��ġ�� ������� ī�޶� ȸ���� �ݿ���
    float3 pos3 = mul(localPosition.xyz, (float3x3)View);
    float4 pos4 = mul(float4(pos3, 1.f), Projection);
    output.PositionProj = pos4;
    return output;
}