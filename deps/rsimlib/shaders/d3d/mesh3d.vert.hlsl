cbuffer MeshParameters : register(b0)
{
    float4x4 mvp;
    // Normal-matrix rows; w holds the tint colour. A negative normal_rows[0].w selects terrain shading, where the
    // rows carry: [1] xyz: eye (mesh space), w: morph start. [2] x: morph end. D3D12 has no texture arrays, so
    // terrain is drawn in its vertex colours.
    float4 normal_rows[3];
    float4 light;
    float4 light_colour;
    float4 fog_colour;
    float4 fog_beta;
    float4 fog_view;
};

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
    float4 tangent : TANGENT;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
    float4 tangent : TEXCOORD1;
    // x: -1 for ordinary meshes, 0 for terrain.
    float4 terrain : TEXCOORD2;
    // View-space depth, for fog.
    float view_depth : TEXCOORD3;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    if (normal_rows[0].w < 0.0)
    {
        float3 eye = normal_rows[1].xyz;
        float morph = smoothstep(normal_rows[1].w, normal_rows[2].x, distance(input.position, eye));
        float3 position = input.position + input.tangent.xyz * morph;
        output.position = mul(mvp, float4(position, 1.0));
        output.view_depth = output.position.w;
        output.position.z = output.position.z * 0.5 + output.position.w * 0.5;
        output.normal = input.normal;
        output.color = input.color;
        output.tangent = float4(input.position, 1.0);
        output.texcoord = input.texcoord;
        output.terrain = float4(0.0, 0.0, 0.0, 0.0);
        return output;
    }
    output.position = mul(mvp, float4(input.position, 1.0));
    output.view_depth = output.position.w;
    output.position.z = output.position.z * 0.5 + output.position.w * 0.5;
    output.normal = float3(dot(normal_rows[0].xyz, input.normal),
                           dot(normal_rows[1].xyz, input.normal),
                           dot(normal_rows[2].xyz, input.normal));
    output.color = float4(input.color.rgb * float3(normal_rows[0].w, normal_rows[1].w, normal_rows[2].w), input.color.a);
    output.tangent = float4(dot(normal_rows[0].xyz, input.tangent.xyz),
                            dot(normal_rows[1].xyz, input.tangent.xyz),
                            dot(normal_rows[2].xyz, input.tangent.xyz), input.tangent.w);
    output.texcoord = input.texcoord;
    output.terrain = float4(-1.0, 0.0, 0.0, 0.0);
    return output;
}
