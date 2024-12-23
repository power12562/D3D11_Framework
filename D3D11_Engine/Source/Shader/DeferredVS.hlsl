struct VS_INPUT
{
    float4 position : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_Deferred
{
    float4 svposition : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

PS_Deferred main(VS_INPUT input)
{
    PS_Deferred output;
    output.svposition = input.position;
    output.Tex = input.Tex;
    
    return output;
}