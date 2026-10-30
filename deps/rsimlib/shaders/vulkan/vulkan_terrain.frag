#version 450
layout(location = 0) in vec3 vNormal;
layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec4 vWeightsA;
layout(location = 3) in vec4 vWeightsB;
layout(location = 4) in vec2 vUv;
layout(location = 5) in float vFade;
layout(location = 6) in float vViewDepth;
layout(set = 0, binding = 0) uniform sampler2DArray uAlbedo;
layout(set = 1, binding = 0) uniform sampler2DArray uNormals;
layout(push_constant) uniform Light { layout(offset = 112) vec4 light; } lighting;
layout(set = 2, binding = 0) uniform Environment
{
    vec4 lightColour;
    vec4 fogColour;
    vec4 fogBeta;
    vec4 fogView;
    vec4 layers[8];
    vec4 average[8];
    vec4 macro;
    // Chunk-local eye position in xyz (the world origin of lights and shadows) and the local light count in w.
    vec4 cameraPosition;
    vec4 shadow;
    mat4 shadowMatrix;
    // x is 1 when the material is pbr (specular highlights on); z is the roughness.
    vec4 pbr;
    vec4 locals[160];
} environment;
layout(set = 4, binding = 0) uniform sampler2D uShadowMap;
layout(set = 5, binding = 0) uniform sampler2D uLocalShadowMap;
layout(location = 0) out vec4 fragColor;

const float PI = 3.14159265359;

// Cook-Torrance (GGX, Smith-Schlick, Schlick Fresnel) specular only, times the light's N.L, for a dielectric.
vec3 pbr_specular(vec3 N, vec3 V, vec3 L, float roughness)
{
    vec3 F0 = vec3(0.04);
    vec3 H = normalize(L + V);
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
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdH, 5.0);
    return D * G * F * PI / (4.0 * NdV);
}

uint hash3(uvec3 v)
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
    float x = clamp(distance_to_light / range, 0.0, 1.0);
    float window = 1.0 - x * x * x * x;
    return window * window / (distance_to_light * distance_to_light + 1.0);
}

float shadow_depth(ivec2 texel)
{
    vec3 c = texelFetch(uShadowMap, texel, 0).rgb;
    return c.r + c.g / 255.0 + c.b / 65025.0;
}

// Fraction of the sun that reaches `world` (1 lit, 0 shadowed): 5x5 percentage-closer filtering with a bilinear weight.
float sun_visibility(vec3 world, vec3 normal)
{
    vec4 clip = environment.shadowMatrix * vec4(world + normal * (environment.shadow.z * 1.5), 1.0);
    vec3 coord = clip.xyz * 0.5 + 0.5;
    if (coord.x < 0.0 || coord.x > 1.0 || coord.y < 0.0 || coord.y > 1.0 || coord.z > 1.0)
        return 1.0;
    ivec2 size = textureSize(uShadowMap, 0);
    // The viewport is flipped, so texture row 0 is the top of clip space.
    vec2 pixel = vec2(coord.x, 1.0 - coord.y) * vec2(size) - 0.5;
    ivec2 base = ivec2(floor(pixel));
    vec2 f = pixel - vec2(base);
    float reference = coord.z - environment.shadow.y;
    float sum = 0.0;
    float total = 0.0;
    for (int y = -1; y <= 2; ++y)
        for (int x = -1; x <= 2; ++x)
        {
            ivec2 t = clamp(base + ivec2(x, y), ivec2(0), size - 1);
            float w = (x == -1 ? 1.0 - f.x : (x == 2 ? f.x : 1.0)) * (y == -1 ? 1.0 - f.y : (y == 2 ? f.y : 1.0));
            sum += w * (reference <= shadow_depth(t) ? 1.0 : 0.0);
            total += w;
        }
    return sum / total;
}
// Local light shadow atlas: 3 faces across, 2 per slot down. Matrices follow the lights in the environment locals from vec4 64.
float local_depth(ivec2 texel)
{
    vec3 c = texelFetch(uLocalShadowMap, texel, 0).rgb;
    return c.r + c.g / 255.0 + c.b / 65025.0;
}

float local_visibility(vec3 world, vec3 normal, vec3 light_position, bool spot, float slot, float near_plane, float far_plane)
{
    vec3 d = world - light_position;
    float dist = length(d);
    int face = 0;
    if (!spot)
    {
        vec3 a = abs(d);
        if (a.x >= a.y && a.x >= a.z)
            face = d.x > 0.0 ? 0 : 1;
        else if (a.y >= a.z)
            face = d.y > 0.0 ? 2 : 3;
        else
            face = d.z > 0.0 ? 4 : 5;
    }
    int size = textureSize(uLocalShadowMap, 0).x / 3;
    float texel_world = 2.0 * dist / float(size);
    vec4 clip = mat4(environment.locals[64 + (int(slot) * 6 + face) * 4], environment.locals[65 + (int(slot) * 6 + face) * 4],
                     environment.locals[66 + (int(slot) * 6 + face) * 4], environment.locals[67 + (int(slot) * 6 + face) * 4]) *
                vec4(world + normal * (texel_world * 1.5), 1.0);
    if (clip.w <= 0.0)
        return 1.0;
    vec3 coord = clip.xyz / clip.w * 0.5 + 0.5;
    if (coord.x < 0.0 || coord.x > 1.0 || coord.y < 0.0 || coord.y > 1.0 || coord.z > 1.0)
        return 1.0;
    float reference = near_plane * far_plane / (far_plane - coord.z * (far_plane - near_plane)) - texel_world * 1.5;
    ivec2 origin = ivec2((face % 3) * size, (int(slot) * 2 + face / 3) * size);
    vec2 pixel = vec2(coord.x, (1.0 - coord.y)) * float(size) - 0.5;
    ivec2 base = ivec2(floor(pixel));
    vec2 f = pixel - vec2(base);
    float sum = 0.0;
    float total = 0.0;
    for (int y = -1; y <= 2; ++y)
        for (int x = -1; x <= 2; ++x)
        {
            ivec2 t = clamp(base + ivec2(x, y), ivec2(0), ivec2(size - 1));
            float stored = local_depth(origin + t);
            float linear_depth = near_plane * far_plane / (far_plane - stored * (far_plane - near_plane));
            float w = (x == -1 ? 1.0 - f.x : (x == 2 ? f.x : 1.0)) * (y == -1 ? 1.0 - f.y : (y == 2 ? f.y : 1.0));
            sum += w * (reference <= linear_depth ? 1.0 : 0.0);
            total += w;
        }
    return sum / total;
}

void main()
{
    if (environment.pbr.w > 0.5)
    {
        // Render style: 1 flat shaded, 2 wireframe. Vertex colour or layer average, no textures, fog or specular.
        vec3 face = normalize(cross(dFdx(vPosition), dFdy(vPosition)));
        if (dot(face, vPosition - environment.cameraPosition.xyz) > 0.0)
            face = -face;
        vec3 base = vWeightsA.rgb;
        if (environment.macro.z >= 0.0)
        {
            float total = 0.0;
            float w[8] = float[8](vWeightsA.x, vWeightsA.y, vWeightsA.z, vWeightsA.w, vWeightsB.x, vWeightsB.y, vWeightsB.z, vWeightsB.w);
            base = vec3(0.0);
            for (int i = 0; i < 8; ++i)
            {
                base += w[i] * environment.average[i].rgb;
                total += w[i];
            }
            base /= max(total, 1e-4);
        }
        float lit = environment.pbr.w > 1.5 ? 1.5 : lighting.light.w + (1.0 - lighting.light.w) * max(dot(face, lighting.light.xyz), 0.0);
        fragColor = vec4(base * lit, 1.0);
        return;
    }
    vec3 normal = normalize(vNormal);
    vec2 uv_dx = dFdx(vUv);
    vec2 uv_dy = dFdy(vUv);
    vec3 dp1 = dFdx(vPosition);
    vec3 dp2 = dFdy(vPosition);
    float handed = dot(cross(dp1, dp2), normal) < 0.0 ? -1.0 : 1.0;
    vec3 dp2perp = cross(dp2, normal);
    vec3 dp1perp = cross(normal, dp1);
    vec3 tangent = (dp2perp * uv_dx.x + dp1perp * uv_dy.x) * handed;
    vec3 bitangent = (dp2perp * uv_dx.y + dp1perp * uv_dy.y) * handed;
    float frame_scale = inversesqrt(max(max(dot(tangent, tangent), dot(bitangent, bitangent)), 1e-20));
    tangent *= frame_scale;
    bitangent *= frame_scale;

    float weights[8] = float[8](vWeightsA.x, vWeightsA.y, vWeightsA.z, vWeightsA.w,
                                vWeightsB.x, vWeightsB.y, vWeightsB.z, vWeightsB.w);
    bool has_normals = environment.macro.w > 0.5;
    vec3 albedo = vec3(0.0);
    vec2 slope = vec2(0.0);
    float total = 0.0;
    float gloss = 0.0;
    // A negative macro strength is the colour-detail mode: the vertex colour is the albedo, its alpha the gloss.
    bool colour_mode = environment.macro.z < 0.0;
    for (int layer = 0; layer < 8 && !colour_mode; ++layer)
    {
        float weight = weights[layer];
        if (weight < 0.004)
            continue;
        vec4 info = environment.layers[layer];
        vec2 st = vUv / info.z;
        vec2 cell = mod(floor(st), info.w);
        int variant = int(hash3(uvec3(uvec2(cell), uint(layer))) % uint(info.y));
        vec3 coord = vec3(st, info.x + float(variant));
        vec2 grad_x = uv_dx / info.z;
        vec2 grad_y = uv_dy / info.z;
        vec3 sampled = textureGrad(uAlbedo, coord, grad_x, grad_y).rgb;
        albedo += weight * mix(sampled, environment.average[layer].rgb, vFade);
        total += weight;
        gloss += weight * environment.average[layer].a;
        if (has_normals)
        {
            vec2 n = textureGrad(uNormals, coord, grad_x, grad_y).xy * 2.0 - 1.0;
            slope += weight * n * (1.0 - vFade);
        }
    }
    if (colour_mode)
    {
        albedo = vWeightsA.rgb;
        gloss = vWeightsA.a;
    }
    else
    {
        albedo /= max(total, 1e-4);
        gloss /= max(total, 1e-4);
        slope /= max(total, 1e-4);
    }

    if (environment.macro.x >= 0.0)
    {
        vec2 macro_uv = vUv / environment.macro.y;
        vec3 macro_colour = textureGrad(uAlbedo, vec3(macro_uv, environment.macro.x),
                                        uv_dx / environment.macro.y, uv_dy / environment.macro.y).rgb;
        float brightness = dot(macro_colour, vec3(0.299, 0.587, 0.114));
        if (colour_mode)
        {
            // Three octaves at whole-number repeats (so chunks still agree): blotches, clods and grain.
            brightness = 0.0;
            float octave_weight[3] = float[3](0.4, 0.35, 0.25);
            float octave_scale[3] = float[3](1.0, 4.0, 16.0);
            for (int o = 0; o < 3; ++o)
            {
                vec3 c = textureGrad(uAlbedo, vec3(macro_uv * octave_scale[o], environment.macro.x),
                                     uv_dx * octave_scale[o] / environment.macro.y, uv_dy * octave_scale[o] / environment.macro.y).rgb;
                brightness += octave_weight[o] * dot(c, vec3(0.299, 0.587, 0.114));
            }
            albedo *= max(1.0 + (-environment.macro.z) * (1.0 - vFade) * (brightness - 0.5) * 3.0, 0.0);
        }
        else
            albedo *= max(1.0 + environment.macro.z * (brightness - 0.5) * 2.0, 0.0);
    }

    if (has_normals)
    {
        vec3 mapped = vec3(slope, sqrt(max(1.0 - dot(slope, slope), 0.0)));
        normal = normalize(tangent * mapped.x - bitangent * mapped.y + normal * mapped.z);
    }

    vec3 world = vPosition - environment.cameraPosition.xyz;
    vec3 sun_rgb = environment.lightColour.rgb;
    if (environment.shadow.x > 0.5 && lighting.light.w < 1.0)
        sun_rgb *= sun_visibility(world, normalize(vNormal));
    float diffuse = max(dot(normal, lighting.light.xyz), 0.0);
    vec3 colour = albedo * (lighting.light.w + (1.0 - lighting.light.w) * diffuse * sun_rgb);
    // The shader's world space is eye-relative, so the view direction points back at the origin.
    vec3 view = normalize(-world);
    float roughness = clamp(environment.pbr.z, 0.04, 1.0);
    bool glossy = environment.pbr.x > 0.5 && gloss > 0.003;
    if (glossy)
        colour += gloss * pbr_specular(normal, view, lighting.light.xyz, roughness) * sun_rgb * (1.0 - lighting.light.w);
    int local_count = int(environment.cameraPosition.w);
    for (int i = 0; i < local_count; ++i)
    {
        vec4 position = environment.locals[i * 4];
        vec4 light_colour = environment.locals[i * 4 + 1];
        vec4 spot = environment.locals[i * 4 + 2];
        vec4 shadow_info = environment.locals[i * 4 + 3];
        vec3 to_light = position.xyz - world;
        float distance_to_light = length(to_light);
        if (distance_to_light >= position.w)
            continue;
        vec3 L = to_light / max(distance_to_light, 1e-4);
        float strength = local_attenuation(distance_to_light, position.w);
        if (light_colour.w > -0.5)
            strength *= clamp((dot(-L, spot.xyz) - light_colour.w) * spot.w, 0.0, 1.0);
        if (shadow_info.x > -0.5)
            strength *= local_visibility(world, normalize(vNormal), position.xyz, light_colour.w > -0.5, shadow_info.x, shadow_info.y, shadow_info.z);
        vec3 reflected = albedo * max(dot(normal, L), 0.0);
        if (glossy)
            reflected += gloss * pbr_specular(normal, view, L, roughness);
        colour += reflected * light_colour.rgb * strength;
    }
    if (environment.fogBeta.r + environment.fogBeta.g + environment.fogBeta.b > 0.0)
    {
        vec2 ndc = gl_FragCoord.xy * environment.fogView.zw * 2.0 - 1.0;
        float distance_to_eye = vViewDepth * length(vec3(ndc * environment.fogView.xy, 1.0));
        colour = mix(environment.fogColour.rgb, colour, exp(-environment.fogBeta.rgb * distance_to_eye));
    }
    fragColor = vec4(colour, 1.0);
}
