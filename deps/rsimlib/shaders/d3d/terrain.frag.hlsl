// Splatted terrain: up to eight surface layers blended from texture arrays, with a macro variation texture, distance fade
// to each layer's average colour, normal maps, sun and local-light shadows, and fog. Mirrors terrain.frag (GLSL).
Texture2DArray albedo_array : register(t0);
Texture2DArray normal_array : register(t1);
SamplerState terrain_sampler : register(s0);
// Sun shadow map: packed 24-bit depth in rgb, fetched texel by texel.
Texture2D shadow_value : register(t3);
// Local-light shadow atlas: packed 24-bit depth, fetched texel by texel.
Texture2D local_shadow_value : register(t4);

cbuffer MeshParameters : register(b0)
{
    float4x4 mvp;
    float4 normal_rows[3];
    // xyz: unit vector toward the light; w: ambient.
    float4 light;
    float4 light_colour;
    float4 fog_colour;
    float4 fog_beta;
    float4 fog_view;
};

cbuffer TerrainParameters : register(b1)
{
    // x: 1 when specular highlights are on; z: roughness.
    float4 pbr;
    float4 emissive;
    float4 camera_right;
    float4 camera_up;
    // w: render style (1 flat, 2 wireframe).
    float4 camera_forward;
    // xyz: the eye in the mesh's chunk-local space (the origin of lights and shadows); w: local light count.
    float4 camera_position;
    // x: 0 off, 1 receive shadows; y: depth bias; z: texel world size.
    float4 shadow;
    float4x4 shadow_matrix;
    float4 locals[160];
    // Per layer: x first array layer, y variant count, z tile metres, w tiles per UV period.
    float4 layers[8];
    // Per layer: rgb average colour, a gloss.
    float4 average[8];
    // x: macro layer (negative for none), y: macro tile metres, z: macro strength (negative: colour-detail mode),
    // w: 1 when normal maps are present.
    float4 macro;
};

static const float PI = 3.14159265359;

// Cook-Torrance (GGX, Smith-Schlick, Schlick Fresnel) specular only, times the light's N.L, for a dielectric.
float3 pbr_specular(float3 N, float3 V, float3 L, float roughness)
{
    float3 F0 = float3(0.04, 0.04, 0.04);
    float3 H = normalize(L + V);
    float NdL = max(dot(N, L), 0.0);
    float NdV = max(dot(N, V), 1e-4);
    float NdH = max(dot(N, H), 0.0);
    float VdH = max(dot(V, H), 0.0);
    float a = roughness * roughness;
    float a2 = a * a;
    float d = NdH * NdH * (a2 - 1.0) + 1.0;
    float D = a2 / (PI * d * d);
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    float G = (NdV / (NdV * (1.0 - k) + k)) * (NdL / (NdL * (1.0 - k) + k));
    float3 F = F0 + (1.0 - F0) * pow(1.0 - VdH, 5.0);
    return D * G * F * PI / (4.0 * NdV);
}

uint hash3(uint3 v)
{
    v = v * 1664525u + 1013904223u;
    v.x += v.y * v.z;
    v.y += v.z * v.x;
    v.z += v.x * v.y;
    v ^= v >> 16u;
    v.x += v.y * v.z;
    v.y += v.z * v.x;
    v.z += v.x * v.y;
    return v.x ^ v.y ^ v.z;
}

// Smooth distance window: 1 at the light, exactly 0 at its range, with an inverse-square core.
float local_attenuation(float distance_to_light, float range)
{
    float x = saturate(distance_to_light / range);
    float window = 1.0 - x * x * x * x;
    return window * window / (distance_to_light * distance_to_light + 1.0);
}

float shadow_depth(int2 texel)
{
    float3 c = shadow_value.Load(int3(texel, 0)).rgb;
    return c.r + c.g / 255.0 + c.b / 65025.0;
}

float local_shadow_depth(int2 texel)
{
    float3 c = local_shadow_value.Load(int3(texel, 0)).rgb;
    return c.r + c.g / 255.0 + c.b / 65025.0;
}

// Fraction of the sun that reaches `world` (1 lit, 0 shadowed): 5x5 percentage-closer filtering with a bilinear weight.
float sun_visibility(float3 world, float3 normal)
{
    float4 clip = mul(shadow_matrix, float4(world + normal * (shadow.z * 1.5), 1.0));
    float3 coord = clip.xyz * 0.5 + 0.5;
    if (coord.x < 0.0 || coord.x > 1.0 || coord.y < 0.0 || coord.y > 1.0 || coord.z > 1.0)
        return 1.0;
    uint width, height;
    shadow_value.GetDimensions(width, height);
    int2 size = int2(width, height);
    // Framebuffer y grows downward, so texture row 0 is the top of clip space.
    float2 pixel = float2(coord.x, 1.0 - coord.y) * float2(size) - 0.5;
    int2 base = (int2)floor(pixel);
    float2 f = pixel - float2(base);
    float reference = coord.z - shadow.y;
    float sum = 0.0;
    float total = 0.0;
    for (int y = -1; y <= 2; ++y)
        for (int x = -1; x <= 2; ++x)
        {
            int2 t = clamp(base + int2(x, y), int2(0, 0), size - 1);
            float w = (x == -1 ? 1.0 - f.x : (x == 2 ? f.x : 1.0)) * (y == -1 ? 1.0 - f.y : (y == 2 ? f.y : 1.0));
            sum += w * (reference <= shadow_depth(t) ? 1.0 : 0.0);
            total += w;
        }
    return sum / total;
}

float local_visibility(float3 world, float3 normal, float3 light_position, bool spot, int slot,
                       float near_plane, float far_plane)
{
    float3 d = world - light_position;
    float dist = length(d);
    int face = 0;
    if (!spot)
    {
        float3 a = abs(d);
        if (a.x >= a.y && a.x >= a.z)
            face = d.x > 0.0 ? 0 : 1;
        else if (a.y >= a.z)
            face = d.y > 0.0 ? 2 : 3;
        else
            face = d.z > 0.0 ? 4 : 5;
    }
    uint width, height;
    local_shadow_value.GetDimensions(width, height);
    int size = int(width / 3);
    float texel_world = 2.0 * dist / float(size);
    int matrix_index = 64 + (slot * 6 + face) * 4;
    float4 world_position = float4(world + normal * (texel_world * 1.5), 1.0);
    float4 clip = locals[matrix_index] * world_position.x +
                  locals[matrix_index + 1] * world_position.y +
                  locals[matrix_index + 2] * world_position.z +
                  locals[matrix_index + 3] * world_position.w;
    if (clip.w <= 0.0)
        return 1.0;
    float3 coord = clip.xyz / clip.w * 0.5 + 0.5;
    if (coord.x < 0.0 || coord.x > 1.0 || coord.y < 0.0 || coord.y > 1.0 || coord.z > 1.0)
        return 1.0;
    float reference = near_plane * far_plane / (far_plane - coord.z * (far_plane - near_plane)) -
                      texel_world * 1.5;
    int2 origin = int2((face % 3) * size, (slot * 2 + face / 3) * size);
    float2 pixel = float2(coord.x, 1.0 - coord.y) * float(size) - 0.5;
    int2 base = int2(floor(pixel));
    float2 f = pixel - float2(base);
    float sum = 0.0;
    float total = 0.0;
    for (int y = -1; y <= 2; ++y)
        for (int x = -1; x <= 2; ++x)
        {
            int2 t = clamp(base + int2(x, y), int2(0, 0), int2(size - 1, size - 1));
            float stored = local_shadow_depth(origin + t);
            float linear_depth = near_plane * far_plane / (far_plane - stored * (far_plane - near_plane));
            float w = (x == -1 ? 1.0 - f.x : (x == 2 ? f.x : 1.0)) *
                      (y == -1 ? 1.0 - f.y : (y == 2 ? f.y : 1.0));
            sum += w * (reference <= linear_depth ? 1.0 : 0.0);
            total += w;
        }
    return sum / total;
}

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 world : TEXCOORD0;
    float4 weights_a : COLOR0;
    float4 weights_b : TEXCOORD1;
    float2 texcoord : TEXCOORD2;
    float fade : TEXCOORD3;
    float view_depth : TEXCOORD4;
};

float4 main(PixelInput input) : SV_TARGET
{
    float weights[8] = {input.weights_a.x, input.weights_a.y, input.weights_a.z, input.weights_a.w,
                        input.weights_b.x, input.weights_b.y, input.weights_b.z, input.weights_b.w};
    // Derivatives are taken before any divergent control flow.
    float2 uv_dx = ddx(input.texcoord);
    float2 uv_dy = ddy(input.texcoord);
    float3 dp1 = ddx(input.world);
    float3 dp2 = ddy(input.world);

    if (camera_forward.w > 0.5)
    {
        // Render style: 1 flat shaded, 2 wireframe. Vertex colour or layer average, no textures, fog or specular.
        float3 face = normalize(cross(dp1, dp2));
        if (dot(face, input.world - camera_position.xyz) > 0.0)
            face = -face;
        float3 base = input.weights_a.rgb;
        if (macro.z >= 0.0)
        {
            float total = 0.0;
            base = float3(0.0, 0.0, 0.0);
            for (int i = 0; i < 8; ++i)
            {
                base += weights[i] * average[i].rgb;
                total += weights[i];
            }
            base /= max(total, 1e-4);
        }
        float lit = camera_forward.w > 1.5 ? 1.5 : light.w + (1.0 - light.w) * max(dot(face, light.xyz), 0.0);
        return float4(base * lit, 1.0);
    }

    float3 normal = normalize(input.normal);
    // The view's y axis is flipped relative to GL, which flips every derivative along y; the frame below is unchanged
    // because the position and UV derivatives flip together.
    float handed = dot(cross(dp1, dp2), normal) < 0.0 ? -1.0 : 1.0;
    float3 dp2perp = cross(dp2, normal);
    float3 dp1perp = cross(normal, dp1);
    float3 tangent = (dp2perp * uv_dx.x + dp1perp * uv_dy.x) * handed;
    float3 bitangent = (dp2perp * uv_dx.y + dp1perp * uv_dy.y) * handed;
    float frame_scale = rsqrt(max(max(dot(tangent, tangent), dot(bitangent, bitangent)), 1e-20));
    tangent *= frame_scale;
    bitangent *= frame_scale;

    bool has_normals = macro.w > 0.5;
    float3 albedo = float3(0.0, 0.0, 0.0);
    float2 slope = float2(0.0, 0.0);
    float total = 0.0;
    float gloss = 0.0;
    // A negative macro strength is the colour-detail mode: the vertex colour is the albedo, its alpha the gloss.
    bool colour_mode = macro.z < 0.0;
    if (!colour_mode)
    {
        [loop] for (int layer = 0; layer < 8; ++layer)
        {
            float weight = weights[layer];
            if (weight < 0.004)
                continue;
            float4 info = layers[layer];
            float2 st = input.texcoord / info.z;
            float2 cell = st - info.w * floor(st / info.w);
            uint variant = hash3(uint3(uint2(cell), uint(layer))) % uint(info.y);
            float3 coord = float3(st, info.x + float(variant));
            float2 grad_x = uv_dx / info.z;
            float2 grad_y = uv_dy / info.z;
            float3 sampled = albedo_array.SampleGrad(terrain_sampler, coord, grad_x, grad_y).rgb;
            albedo += weight * lerp(sampled, average[layer].rgb, input.fade);
            total += weight;
            gloss += weight * average[layer].a;
            if (has_normals)
            {
                float2 n = normal_array.SampleGrad(terrain_sampler, coord, grad_x, grad_y).xy * 2.0 - 1.0;
                slope += weight * n * (1.0 - input.fade);
            }
        }
        albedo /= max(total, 1e-4);
        gloss /= max(total, 1e-4);
        slope /= max(total, 1e-4);
    }
    else
    {
        albedo = input.weights_a.rgb;
        gloss = input.weights_a.a;
    }

    if (macro.x >= 0.0)
    {
        float2 macro_uv = input.texcoord / macro.y;
        float brightness = 0.0;
        if (colour_mode)
        {
            // Three octaves at whole-number repeats (so chunks still agree): blotches, clods and grain.
            float octave_weight[3] = {0.4, 0.35, 0.25};
            float octave_scale[3] = {1.0, 4.0, 16.0};
            for (int o = 0; o < 3; ++o)
            {
                float3 c = albedo_array.SampleGrad(terrain_sampler, float3(macro_uv * octave_scale[o], macro.x),
                                                   uv_dx * octave_scale[o] / macro.y,
                                                   uv_dy * octave_scale[o] / macro.y).rgb;
                brightness += octave_weight[o] * dot(c, float3(0.299, 0.587, 0.114));
            }
            albedo *= max(1.0 + (-macro.z) * (1.0 - input.fade) * (brightness - 0.5) * 3.0, 0.0);
        }
        else
        {
            float3 macro_colour = albedo_array.SampleGrad(terrain_sampler, float3(macro_uv, macro.x),
                                                          uv_dx / macro.y, uv_dy / macro.y).rgb;
            brightness = dot(macro_colour, float3(0.299, 0.587, 0.114));
            albedo *= max(1.0 + macro.z * (brightness - 0.5) * 2.0, 0.0);
        }
    }

    if (has_normals)
    {
        float3 mapped = float3(slope, sqrt(max(1.0 - dot(slope, slope), 0.0)));
        normal = normalize(tangent * mapped.x - bitangent * mapped.y + normal * mapped.z);
    }

    float3 world = input.world - camera_position.xyz;
    float3 sun_rgb = light_colour.rgb;
    if (shadow.x > 0.5 && light.w < 1.0)
        sun_rgb *= sun_visibility(world, normalize(input.normal));
    float diffuse = max(dot(normal, light.xyz), 0.0);
    float3 colour = albedo * (light.w + (1.0 - light.w) * diffuse * sun_rgb);
    // World space is eye-relative, so the view direction points back at the origin.
    float3 view = normalize(-world);
    float roughness = clamp(pbr.z, 0.04, 1.0);
    bool glossy = pbr.x > 0.5 && gloss > 0.003;
    if (glossy)
        colour += gloss * pbr_specular(normal, view, light.xyz, roughness) * sun_rgb * (1.0 - light.w);
    int local_count = int(camera_position.w);
    [loop] for (int i = 0; i < local_count; ++i)
    {
        float4 position = locals[i * 4];
        float4 local_colour = locals[i * 4 + 1];
        float4 spot = locals[i * 4 + 2];
        float4 shadow_info = locals[i * 4 + 3];
        float3 to_light = position.xyz - world;
        float distance_to_light = length(to_light);
        if (distance_to_light >= position.w)
            continue;
        float3 L = to_light / max(distance_to_light, 1e-4);
        float strength = local_attenuation(distance_to_light, position.w);
        if (local_colour.w > -0.5)
            strength *= saturate((dot(-L, spot.xyz) - local_colour.w) * spot.w);
        if (shadow_info.x > -0.5)
            strength *= local_visibility(world, normalize(input.normal), position.xyz, local_colour.w > -0.5,
                                         int(shadow_info.x), shadow_info.y, shadow_info.z);
        float3 reflected = albedo * max(dot(normal, L), 0.0);
        if (glossy)
            reflected += gloss * pbr_specular(normal, view, L, roughness);
        colour += reflected * local_colour.rgb * strength;
    }
    if (fog_beta.r + fog_beta.g + fog_beta.b > 0.0)
    {
        float2 ndc = input.position.xy * fog_view.zw * 2.0 - 1.0;
        float distance_to_eye = input.view_depth * length(float3(ndc * fog_view.xy, 1.0));
        colour = lerp(fog_colour.rgb, colour, exp(-fog_beta.rgb * distance_to_eye));
    }
    return float4(colour, 1.0);
}
