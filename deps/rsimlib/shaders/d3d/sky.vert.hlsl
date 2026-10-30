// A fullscreen triangle at the far plane. vNdc interpolates to [-1, 1] over the screen.
struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 ndc : TEXCOORD0;
};

VertexOutput main(uint vertex_id : SV_VertexID)
{
    VertexOutput output;
    float2 corner = float2(float((vertex_id << 1) & 2), float(vertex_id & 2));
    output.ndc = corner * 2.0 - 1.0;
    output.position = float4(output.ndc, 1.0, 1.0);
    return output;
}
