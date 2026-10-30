#version 430 core

in vec3 vNormal;
in vec4 vTangent;
in vec4 vColour;
in vec2 vUv;
in float vTerrain;
in float vViewDepth;

uniform sampler2D uTexture;
uniform sampler2D uNormalMap;
uniform sampler2D uMaterial;
// Sun shadow map: packed 24-bit depth in rgb, fetched texel by texel.
uniform sampler2D uShadowMap;
// x: 0 off, 1 receive shadows, 2 shadow-pass draw (writes packed depth); y: depth bias; z: texel world size.
uniform vec4 uShadow;
uniform mat4 uShadowMatrix;

// xyz: unit vector toward the light, w: ambient. Unlit draws use (0, 0, 0, 1).
uniform vec4 uLight;
// rgb: colour of the directional light.
uniform vec4 uLightColour;
// Fog: colour fogged surfaces fade to, per-channel extinction per metre, and (tan x, tan y, 1/width, 1/height) of the
// view, which turn view depth into distance along the ray.
uniform vec4 uFogColour;
uniform vec4 uFogBeta;
uniform vec4 uFogView;

// PBR (x = 1 when enabled): metallic, roughness, occlusion strength in yzw. Emissive rgb. Camera right/up (scaled by the
// field of view) and forward in world space.
uniform vec4 uPbr;
uniform vec4 uEmissive;
uniform vec4 uCameraRight;
uniform vec4 uCameraUp;
uniform vec4 uCameraForward;

// World-space eye position (xyz) and the number of local lights (w); three vec4s per light, see MeshDrawParams.
uniform vec4 uCameraPosition;
uniform float uStyle;
uniform vec4 uLocals[160];
uniform sampler2D uLocalShadowMap;

out vec4 fragColor;

const float PI = 3.14159265359;

// Cook-Torrance (GGX, Smith-Schlick, Schlick Fresnel): the reflected fraction of light arriving along L, times N.L.
vec3 pbr_direct(vec3 N, vec3 V, vec3 L, vec3 albedo, float metallic, float roughness)
{
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
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
    vec3 specular = D * G * F / (4.0 * NdV * max(NdL, 1e-4));
    vec3 diffuse = (1.0 - F) * (1.0 - metallic) * albedo;
    return (diffuse + specular * PI) * NdL;
}

// The directional light plus an ambient term.
// Light colour and ambient follow the unlit-looking scale of the Lambert path: white light gives albedo at normal incidence.
vec3 shade_pbr(vec3 N, vec3 V, vec3 L, vec3 albedo, float metallic, float roughness, float occlusion,
               float ambient, vec3 lightColour)
{
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float NdV = max(dot(N, V), 1e-4);
    vec3 direct = pbr_direct(N, V, L, albedo, metallic, roughness) * lightColour * (1.0 - ambient);
    vec3 Fa = F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - NdV, 5.0);
    vec3 indirect = ambient * occlusion * ((1.0 - Fa) * (1.0 - metallic) * albedo + Fa * (1.0 - 0.5 * roughness));
    return direct + indirect;
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
    vec4 clip = uShadowMatrix * vec4(world + normal * (uShadow.z * 1.5), 1.0);
    vec3 coord = clip.xyz * 0.5 + 0.5;
    if (coord.x < 0.0 || coord.x > 1.0 || coord.y < 0.0 || coord.y > 1.0 || coord.z > 1.0)
        return 1.0;
    ivec2 size = textureSize(uShadowMap, 0);
    vec2 pixel = coord.xy * vec2(size) - 0.5;
    ivec2 base = ivec2(floor(pixel));
    vec2 f = pixel - vec2(base);
    float reference = coord.z - uShadow.y;
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

// Local light shadow atlas: 3 faces across, 2 per slot down. Matrices follow the lights in uLocals from vec4 64.
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
    vec4 clip = mat4(uLocals[64 + (int(slot) * 6 + face) * 4], uLocals[65 + (int(slot) * 6 + face) * 4],
                     uLocals[66 + (int(slot) * 6 + face) * 4], uLocals[67 + (int(slot) * 6 + face) * 4]) *
                vec4(world + normal * (texel_world * 1.5), 1.0);
    if (clip.w <= 0.0)
        return 1.0;
    vec3 coord = clip.xyz / clip.w * 0.5 + 0.5;
    if (coord.x < 0.0 || coord.x > 1.0 || coord.y < 0.0 || coord.y > 1.0 || coord.z > 1.0)
        return 1.0;
    float reference = near_plane * far_plane / (far_plane - coord.z * (far_plane - near_plane)) - texel_world * 1.5;
    ivec2 origin = ivec2((face % 3) * size, (int(slot) * 2 + face / 3) * size);
    vec2 pixel = vec2(coord.x, 1.0 - coord.y) * float(size) - 0.5;
    ivec2 base = ivec2(floor(pixel));
    vec2 f = pixel - vec2(base);
    float sum = 0.0;
    float total = 0.0;
    for (int y = -1; y <= 2; ++y)
        for (int x = -1; x <= 2; ++x)
        {
            ivec2 t = clamp(base + ivec2(x, y), ivec2(0), ivec2(size - 1));
            // Render targets are stored bottom-up in OpenGL, so the atlas rows run the other way.
            ivec2 texel = origin + t;
            texel.y = textureSize(uLocalShadowMap, 0).y - 1 - texel.y;
            float stored = local_depth(texel);
            float linear_depth = near_plane * far_plane / (far_plane - stored * (far_plane - near_plane));
            float w = (x == -1 ? 1.0 - f.x : (x == 2 ? f.x : 1.0)) * (y == -1 ? 1.0 - f.y : (y == 2 ? f.y : 1.0));
            sum += w * (reference <= linear_depth ? 1.0 : 0.0);
            total += w;
        }
    return sum / total;
}

void main()
{
    if (uStyle > 0.5)
    {
        // Cheap style: no textures, fog or specular. Flat uses a face normal from screen derivatives.
        vec2 ndc = gl_FragCoord.xy * uFogView.zw * 2.0 - 1.0;
        vec3 p = (uCameraForward.xyz + ndc.x * uCameraRight.xyz + ndc.y * uCameraUp.xyz) * vViewDepth;
        vec3 face = normalize(cross(dFdx(p), dFdy(p)));
        if (dot(face, p) > 0.0)
            face = -face;
        // A coarse mip keeps the broad colours (continents, hulls) without the cost of full texture detail.
        vec3 base = vTerrain < 0.0 ? vColour.rgb * textureLod(uTexture, vUv, 3.0).rgb : vColour.rgb;
        // Blended draws (glows, stars, haze) would paint over the scene as opaque colour.
        if (vTerrain < 0.0 && (uEmissive.w > 0.0 || (uLightColour.w > 0.0 && vColour.a * texture(uTexture, vUv).a < uLightColour.w)))
            discard;
        if (uStyle > 1.5)
        {
            fragColor = vec4(base * 1.5, 1.0);
            return;
        }
        float lit = uLight.w + (1.0 - uLight.w) * max(dot(face, uLight.xyz), 0.0);
        fragColor = vec4(base * lit, 1.0);
        return;
    }
    vec3 normal = normalize(vNormal);
    vec3 albedo;
    if (vTerrain >= 0.0)
    {
        albedo = vColour.rgb;
    }
    else
    {
        // Tangent-space normal mapping; a zero tangent (no UVs) or a flat map leaves the vertex normal unchanged.
        vec3 tangent = vTangent.xyz - normal * dot(normal, vTangent.xyz);
        if (dot(tangent, tangent) > 1e-8)
        {
            tangent = normalize(tangent);
            vec3 bitangent = cross(normal, tangent) * vTangent.w;
            vec3 mapped = texture(uNormalMap, vUv).xyz * 2.0 - 1.0;
            normal = normalize(tangent * mapped.x + bitangent * mapped.y + normal * mapped.z);
        }
        vec4 sampled = texture(uTexture, vUv);
        albedo = vColour.rgb * sampled.rgb;
        // light colour w is the alpha cutoff of cut-out materials such as foliage and impostors (0 disables it).
        if (uLightColour.w > 0.0 && vColour.a * sampled.a < uLightColour.w)
            discard;
    }
    if (uShadow.x > 1.5)
    {
        float v = gl_FragCoord.z * 255.0;
        float r = floor(v);
        v = (v - r) * 255.0;
        float g = floor(v);
        v = (v - g) * 255.0;
        float b = min(floor(v + 0.5), 255.0);
        fragColor = vec4(r, g, b, 255.0) / 255.0;
        return;
    }
    vec3 colour;
    bool pbr = uPbr.x > 0.5 && vTerrain < 0.0;
    // Untextured terrain carries a specular strength in its vertex alpha, shown when the material is pbr.
    bool gloss = uPbr.x > 0.5 && vTerrain >= 0.0 && vColour.a > 0.003;
    float metallic = 0.0;
    float roughness = 1.0;
    vec3 ray = vec3(0.0);
    if (pbr || gloss || uCameraPosition.w > 0.5 || uShadow.x > 0.5)
    {
        vec2 ndc = gl_FragCoord.xy * uFogView.zw * 2.0 - 1.0;
        ray = uCameraForward.xyz + ndc.x * uCameraRight.xyz + ndc.y * uCameraUp.xyz;
    }
    vec3 sun_rgb = uLightColour.rgb;
    if (uShadow.x > 0.5 && uLight.w < 1.0)
        sun_rgb *= sun_visibility(uCameraPosition.xyz + ray * vViewDepth, normal);
    if (pbr)
    {
        vec3 orm = texture(uMaterial, vUv).rgb;
        metallic = clamp(uPbr.y * orm.b, 0.0, 1.0);
        roughness = clamp(uPbr.z * orm.g, 0.04, 1.0);
        colour = shade_pbr(normal, normalize(-ray), uLight.xyz, albedo, metallic, roughness, mix(1.0, orm.r, uPbr.w),
                           uLight.w, sun_rgb) +
                 uEmissive.rgb;
    }
    else
    {
        float diffuse = max(dot(normal, uLight.xyz), 0.0);
        colour = albedo * (uLight.w + (1.0 - uLight.w) * diffuse * sun_rgb);
    }
    if (gloss)
    {
        roughness = clamp(uPbr.z, 0.04, 1.0);
        colour += vColour.a * pbr_direct(normal, normalize(-ray), uLight.xyz, vec3(0.0), 0.0, roughness) * sun_rgb * (1.0 - uLight.w);
    }
    if (uCameraPosition.w > 0.5 && uLight.w < 1.0)
    {
        vec3 world = uCameraPosition.xyz + ray * vViewDepth;
        vec3 view = normalize(-ray);
        int count = int(uCameraPosition.w);
        for (int i = 0; i < count; ++i)
        {
            vec4 position = uLocals[i * 4];
            vec4 light_colour = uLocals[i * 4 + 1];
            vec4 spot = uLocals[i * 4 + 2];
            vec4 shadow_info = uLocals[i * 4 + 3];
            vec3 to_light = position.xyz - world;
            float distance_to_light = length(to_light);
            if (distance_to_light >= position.w)
                continue;
            vec3 L = to_light / max(distance_to_light, 1e-4);
            float strength = local_attenuation(distance_to_light, position.w);
            if (light_colour.w > -0.5)
                strength *= clamp((dot(-L, spot.xyz) - light_colour.w) * spot.w, 0.0, 1.0);
            if (shadow_info.x > -0.5)
                strength *= local_visibility(world, normal, position.xyz, light_colour.w > -0.5, shadow_info.x, shadow_info.y, shadow_info.z);
            vec3 reflected = pbr ? pbr_direct(normal, view, L, albedo, metallic, roughness)
                                  : albedo * max(dot(normal, L), 0.0);
            if (gloss)
                reflected += vColour.a * pbr_direct(normal, view, L, vec3(0.0), 0.0, roughness);
            colour += reflected * light_colour.rgb * strength;
        }
    }
    if (uFogBeta.r + uFogBeta.g + uFogBeta.b > 0.0)
    {
        vec2 ndc = gl_FragCoord.xy * uFogView.zw * 2.0 - 1.0;
        float distance_to_eye = vViewDepth * length(vec3(ndc * uFogView.xy, 1.0));
        colour = mix(uFogColour.rgb, colour, exp(-uFogBeta.rgb * distance_to_eye));
    }
    // emissive.w > 0 marks a blended draw: its opacity multiplies the vertex and texture alpha.
    float alpha = 1.0;
    if (uEmissive.w > 0.0 && vTerrain < 0.0)
        alpha = clamp(vColour.a * texture(uTexture, vUv).a * uEmissive.w, 0.0, 1.0);
    fragColor = vec4(colour, alpha);
}
