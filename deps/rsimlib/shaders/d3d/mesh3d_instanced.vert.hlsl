cbuffer MeshParameters : register(b0)
{
    // The view-projection; each instance's model matrix is applied in the shader.
    float4x4 mvp;
    // Only w is used: the material tint colour.
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
    // Per-instance model matrix (columns) and tint, from the second vertex stream.
    float4 model0 : INSTANCE0;
    float4 model1 : INSTANCE1;
    float4 model2 : INSTANCE2;
    float4 model3 : INSTANCE3;
    float4 instance_tint : INSTANCE4;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
    float4 tangent : TEXCOORD1;
    float4 terrain : TEXCOORD2;
    float view_depth : TEXCOORD3;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    float4 world = input.model0 * input.position.x + input.model1 * input.position.y +
                   input.model2 * input.position.z + input.model3;
    output.position = mul(mvp, world);
    output.view_depth = output.position.w;
    output.position.z = output.position.z * 0.5 + output.position.w * 0.5;
    // The inverse transpose of the model's 3x3 is its cofactor matrix over the determinant; the pixel shader
    // normalises, so only the determinant's sign (mirrored instances) is needed.
    float3 c0 = input.model0.xyz;
    float3 c1 = input.model1.xyz;
    float3 c2 = input.model2.xyz;
    float3 cof0 = cross(c1, c2);
    float3 cof1 = cross(c2, c0);
    float3 cof2 = cross(c0, c1);
    float handedness = dot(c0, cof0) < 0.0 ? -1.0 : 1.0;
    output.normal = (cof0 * input.normal.x + cof1 * input.normal.y + cof2 * input.normal.z) * handedness;
    output.tangent = float4((cof0 * input.tangent.x + cof1 * input.tangent.y + cof2 * input.tangent.z) * handedness,
                            input.tangent.w);
    output.color = float4(input.color.rgb * float3(normal_rows[0].w, normal_rows[1].w, normal_rows[2].w) *
                              input.instance_tint.rgb,
                          input.color.a * input.instance_tint.a);
    output.texcoord = input.texcoord;
    output.terrain = float4(-1.0, 0.0, 0.0, 0.0);
    return output;
}
