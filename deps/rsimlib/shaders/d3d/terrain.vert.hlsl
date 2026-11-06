cbuffer MeshParameters : register(b0)
{
    float4x4 mvp;
    // Terrain parameters: [0] xyz: quaternion turning the mesh into world axes, [1] xyz: eye (chunk-local), w: morph
    // start, [2] x: morph end, y: fade start, z: fade end, w: quaternion w.
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
    // Weights of terrain layers 0-3, then 4-7 in extra.
    float4 weights_a : COLOR0;
    // Metres along the ground.
    float2 texcoord : TEXCOORD0;
    // Offset to the coarser level's surface.
    float4 tangent : TANGENT;
    float4 weights_b : EXTRA;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 world : TEXCOORD0;
    float4 weights_a : COLOR0;
    float4 weights_b : TEXCOORD1;
    float2 texcoord : TEXCOORD2;
    // 0 near the eye, 1 where the layers have faded completely to their average colour.
    float fade : TEXCOORD3;
    float view_depth : TEXCOORD4;
};

float3 turn_by(float3 v, float4 q)
{
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    float3 eye = normal_rows[1].xyz;
    float4 q = float4(normal_rows[0].xyz, normal_rows[2].w);
    float3 turned = turn_by(input.position, q);
    float morph = smoothstep(normal_rows[1].w, normal_rows[2].x, distance(turned, eye));
    float3 position = turned + turn_by(input.tangent.xyz, q) * morph;
    output.position = mul(mvp, float4(position, 1.0));
    output.view_depth = output.position.w;
    output.position.z = output.position.z * 0.5 + output.position.w * 0.5;
    output.normal = turn_by(input.normal, q);
    output.world = position;
    output.weights_a = input.weights_a;
    output.weights_b = input.weights_b;
    output.texcoord = input.texcoord;
    output.fade = normal_rows[2].z > normal_rows[2].y
                      ? smoothstep(normal_rows[2].y, normal_rows[2].z, distance(position, eye))
                      : 0.0;
    return output;
}
