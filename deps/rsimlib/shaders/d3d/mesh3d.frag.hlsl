Texture2D texture_value : register(t0);
SamplerState texture_sampler : register(s0);
Texture2D normal_value : register(t1);
SamplerState normal_sampler : register(s1);
Texture2D material_value : register(t2);
SamplerState material_sampler : register(s2);
// Sun shadow map: packed 24-bit depth in rgb, fetched texel by texel.
Texture2D shadow_value : register(t3);
// Local-light shadow atlas: packed 24-bit depth, fetched texel by texel.
Texture2D local_shadow_value : register(t4);

cbuffer MeshParameters : register(b0)
{
    float4x4 mvp;
    float4 normal_rows[3];
    float4 light;
    // rgb: colour of the directional light.
    float4 light_colour;
    // Fog: colour fogged surfaces fade to, per-channel extinction per metre, and (tan x, tan y, 1/width, 1/height) of
    // the view, which turn view depth into distance along the ray.
    float4 fog_colour;
    float4 fog_beta;
    float4 fog_view;
};

// PBR (x = 1 when enabled): metallic, roughness, occlusion strength in yzw. Emissive rgb. Camera right/up (scaled by the
// field of view) and forward in world space.
cbuffer PbrParameters : register(b1)
{
    float4 pbr;
    float4 emissive;
    float4 camera_right;
    float4 camera_up;
    float4 camera_forward;
    // World-space eye position (xyz) and the number of local lights (w); four float4s per light, see MeshDrawParams.
    float4 camera_position;
    // x: 0 off, 1 receive shadows, 2 shadow-pass draw (writes packed depth); y: depth bias; z: texel world size.
    float4 shadow;
    float4x4 shadow_matrix;
    float4 locals[160];
};

static const float PI = 3.14159265359;

// Cook-Torrance (GGX, Smith-Schlick, Schlick Fresnel): the reflected fraction of light arriving along L, times N.L.
float3 pbr_direct(float3 N, float3 V, float3 L, float3 albedo, float metallic, float roughness)
{
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
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
    float3 specular = D * G * F / (4.0 * NdV * max(NdL, 1e-4));
    float3 diffuse = (1.0 - F) * (1.0 - metallic) * albedo;
    return (diffuse + specular * PI) * NdL;
}

// The directional light plus an ambient term.
float3 shade_pbr(float3 N, float3 V, float3 L, float3 albedo, float metallic, float roughness, float occlusion,
                 float ambient, float3 light_rgb)
{
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
    float NdV = max(dot(N, V), 1e-4);
    float3 direct = pbr_direct(N, V, L, albedo, metallic, roughness) * light_rgb * (1.0 - ambient);
    float3 Fa = F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - NdV, 5.0);
    float3 indirect = ambient * occlusion * ((1.0 - Fa) * (1.0 - metallic) * albedo + Fa * (1.0 - 0.5 * roughness));
    return direct + indirect;
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
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
    float4 tangent : TEXCOORD1;
    float4 terrain : TEXCOORD2;
    float view_depth : TEXCOORD3;
};

float4 main(PixelInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 albedo;
    float texture_alpha = 1.0;
    if (input.terrain.x >= 0.0)
    {
        albedo = input.color.rgb;
    }
    else
    {
        float3 mapped = normal_value.Sample(normal_sampler, input.texcoord).xyz * 2.0 - 1.0;
        float3 tangent = input.tangent.xyz - normal * dot(normal, input.tangent.xyz);
        if (dot(tangent, tangent) > 1e-8)
        {
            tangent = normalize(tangent);
            float3 bitangent = cross(normal, tangent) * input.tangent.w;
            normal = normalize(tangent * mapped.x + bitangent * mapped.y + normal * mapped.z);
        }
        float4 sampled = texture_value.Sample(texture_sampler, input.texcoord);
        albedo = input.color.rgb * sampled.rgb;
        texture_alpha = sampled.a;
        // light_colour.w is the alpha cutoff of cut-out materials such as foliage and impostors (0 disables it).
        if (light_colour.w > 0.0 && input.color.a * sampled.a < light_colour.w)
            discard;
    }
    if (shadow.x > 1.5)
    {
        float v = input.position.z * 255.0;
        float r = floor(v);
        v = (v - r) * 255.0;
        float g = floor(v);
        v = (v - g) * 255.0;
        float b = min(floor(v + 0.5), 255.0);
        return float4(r, g, b, 255.0) / 255.0;
    }
    if (camera_forward.w > 0.5)
    {
        // Render style: 1 flat shaded, 2 wireframe (the pipeline fills lines). No textures, fog or specular.
        // Blended draws (glows, stars, haze) would paint over the scene as opaque colour.
        if (input.terrain.x < 0.0 && (emissive.w > 0.0 || (light_colour.w > 0.0 && input.color.a * texture_alpha < light_colour.w)))
            discard;
        float2 style_pixel = input.position.xy * fog_view.zw * 2.0;
        float2 style_ndc = float2(style_pixel.x - 1.0, 1.0 - style_pixel.y);
        float3 p = (camera_forward.xyz + style_ndc.x * camera_right.xyz + style_ndc.y * camera_up.xyz) * input.view_depth;
        float3 face = normalize(cross(ddx(p), ddy(p)));
        if (dot(face, p) > 0.0)
            face = -face;
        float lit = camera_forward.w > 1.5 ? 1.5 : light.w + (1.0 - light.w) * max(dot(face, light.xyz), 0.0);
        // A coarse mip keeps the broad colours (continents, hulls) without the cost of full texture detail.
        float3 base = input.terrain.x < 0.0 ? input.color.rgb * texture_value.SampleLevel(texture_sampler, input.texcoord, 3.0).rgb : input.color.rgb;
        return float4(base * lit, 1.0);
    }
    float3 colour;
    bool use_pbr = pbr.x > 0.5 && input.terrain.x < 0.0;
    // Untextured terrain carries a specular strength in its vertex alpha, shown when the material is pbr.
    bool gloss = pbr.x > 0.5 && input.terrain.x >= 0.0 && input.color.a > 0.003;
    float metallic = 0.0;
    float roughness = 1.0;
    float3 ray = float3(0.0, 0.0, 0.0);
    // Framebuffer y grows downward in D3D.
    float2 pixel = input.position.xy * fog_view.zw * 2.0;
    float2 view_ndc = float2(pixel.x - 1.0, 1.0 - pixel.y);
    if (use_pbr || gloss || camera_position.w > 0.5 || shadow.x > 0.5)
    {
        ray = camera_forward.xyz + view_ndc.x * camera_right.xyz + view_ndc.y * camera_up.xyz;
    }
    float3 sun_rgb = light_colour.rgb;
    if (shadow.x > 0.5 && light.w < 1.0)
        sun_rgb *= sun_visibility(camera_position.xyz + ray * input.view_depth, normal);
    if (use_pbr)
    {
        float3 orm = material_value.Sample(material_sampler, input.texcoord).rgb;
        metallic = saturate(pbr.y * orm.b);
        roughness = clamp(pbr.z * orm.g, 0.04, 1.0);
        colour = shade_pbr(normal, normalize(-ray), light.xyz, albedo, metallic, roughness, lerp(1.0, orm.r, pbr.w),
                           light.w, sun_rgb) +
                 emissive.rgb;
    }
    else
    {
        float diffuse = max(dot(normal, light.xyz), 0.0);
        colour = albedo * (light.w + (1.0 - light.w) * diffuse * sun_rgb);
    }
    if (gloss)
    {
        roughness = clamp(pbr.z, 0.04, 1.0);
        colour += input.color.a * pbr_direct(normal, normalize(-ray), light.xyz, float3(0.0, 0.0, 0.0), 0.0, roughness) * sun_rgb * (1.0 - light.w);
    }
    if (camera_position.w > 0.5 && light.w < 1.0)
    {
        float3 world = camera_position.xyz + ray * input.view_depth;
        float3 view = normalize(-ray);
        int count = (int)camera_position.w;
        for (int i = 0; i < count; ++i)
        {
            float4 position = locals[i * 4];
            float4 local_colour = locals[i * 4 + 1];
            float4 spot = locals[i * 4 + 2];
            float3 to_light = position.xyz - world;
            float distance_to_light = length(to_light);
            if (distance_to_light >= position.w)
                continue;
            float3 L = to_light / max(distance_to_light, 1e-4);
            float strength = local_attenuation(distance_to_light, position.w);
            if (local_colour.w > -0.5)
                strength *= saturate((dot(-L, spot.xyz) - local_colour.w) * spot.w);
            float4 shadow_info = locals[i * 4 + 3];
            if (shadow_info.x > -0.5)
                strength *= local_visibility(world, normal, position.xyz, local_colour.w > -0.5,
                                             int(shadow_info.x), shadow_info.y, shadow_info.z);
            float3 reflected = use_pbr ? pbr_direct(normal, view, L, albedo, metallic, roughness)
                                       : albedo * max(dot(normal, L), 0.0);
            if (gloss)
                reflected += input.color.a * pbr_direct(normal, view, L, float3(0.0, 0.0, 0.0), 0.0, roughness);
            colour += reflected * local_colour.rgb * strength;
        }
    }
    if (fog_beta.r + fog_beta.g + fog_beta.b > 0.0)
    {
        float2 ndc = input.position.xy * fog_view.zw * 2.0 - 1.0;
        float distance_to_eye = input.view_depth * length(float3(ndc * fog_view.xy, 1.0));
        colour = lerp(fog_colour.rgb, colour, exp(-fog_beta.rgb * distance_to_eye));
    }
    // emissive.w > 0 marks a blended draw: its opacity multiplies the vertex and texture alpha.
    float alpha = 1.0;
    if (emissive.w > 0.0 && input.terrain.x < 0.0)
        alpha = saturate(input.color.a * texture_alpha * emissive.w);
    return float4(colour, alpha);
}
