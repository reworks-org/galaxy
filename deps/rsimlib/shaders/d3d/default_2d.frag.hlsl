Texture2D texture_value : register(t0);
SamplerState texture_sampler : register(s0);

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PixelInput input) : SV_TARGET
{
    float4 texel = texture_value.Sample(texture_sampler, input.texcoord);
    if (input.color.a < 0.0)
    {
        // Normal pass: color carries an orthogonal 2x2 transform (cos, sin, determinant sign) for the normal's xy.
        float2 v = float2(texel.r * 2.0 - 1.0, -(texel.g * 2.0 - 1.0));
        float det = input.color.b > 0.5 ? 1.0 : -1.0;
        float2 column0 = input.color.rg * 2.0 - 1.0;
        float2 rotated = v.x * column0 + v.y * det * float2(-column0.y, column0.x);
        return float4(float3(rotated.x, -rotated.y, texel.b * 2.0 - 1.0) * 0.5 + 0.5, texel.a);
    }
    return texel * input.color;
}
