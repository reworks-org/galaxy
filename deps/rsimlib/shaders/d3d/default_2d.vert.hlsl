cbuffer Projection : register(b0)
{
    float4x4 projection;
};

struct VertexInput
{
    float2 position : POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    output.position = mul(projection, float4(input.position, 0.0, 1.0));
    output.texcoord = input.texcoord;
    output.color = input.color;
    return output;
}
