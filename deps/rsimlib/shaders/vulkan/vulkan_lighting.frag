#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(set = 0, binding = 1) uniform sampler2D aux[8];
struct GpuLight { vec4 positionRadius; vec4 colourIntensity; vec4 shadowSoftness; };
layout(set = 1, binding = 0, std430) readonly buffer LightBuffer { GpuLight lights[]; } lightBuffer; layout(set = 1, binding = 1, std430) readonly buffer TileCounts { uint tileCounts[]; } tileCountsBuffer; layout(set = 1, binding = 2, std430) readonly buffer TileIndices { uint tileIndices[]; } tileIndicesBuffer;
layout(set = 1, binding = 3, std430) readonly buffer ShadowEdges { vec4 shadowEdges[]; } shadowEdgesBuffer;
layout(push_constant) uniform Lighting { layout(offset = 64) ivec2 tileCount; int lightCount; int shadowLightCount; float ambient; int flipVertical; int useNormals; float lightHeight; } lighting;
layout(location = 0) in vec2 uv; layout(location = 0) out vec4 fragColor;
float visibility(vec2 lightPosition, vec2 target, int first, int count)
{
    vec2 ray = target - lightPosition;
    for (int edge = 0; edge < count; ++edge)
    {
        vec4 segment = shadowEdgesBuffer.shadowEdges[first + edge];
        vec2 start = segment.xy - lightPosition;
        vec2 direction = segment.zw - segment.xy;
        float denominator = ray.x * direction.y - ray.y * direction.x;
        if (abs(denominator) < 1e-6)
            continue;
        float along = (start.x * direction.y - start.y * direction.x) / denominator;
        float across = (start.x * ray.y - start.y * ray.x) / denominator;
        if (along >= 0.0 && along <= 1.0 && across >= 0.0 && across <= 1.0)
            return 0.0;
    }
    return 1.0;
}
void main() { vec2 lightUv = uv; if (lighting.flipVertical != 0) lightUv.y = 1.0 - lightUv.y; vec4 base = texture(source, uv); vec2 pixelPosition = min(lightUv * vec2(textureSize(source, 0)), vec2(textureSize(source, 0) - ivec2(1))); ivec2 tile = ivec2(pixelPosition / 16.0); int tileIndex = tile.y * lighting.tileCount.x + tile.x; uint count = tileCountsBuffer.tileCounts[tileIndex]; vec3 illumination = vec3(lighting.ambient); vec4 normalSample = lighting.useNormals != 0 ? texture(aux[0], uv) : vec4(0.0); bool hasNormal = normalSample.a > 0.5; vec3 surfaceNormal = normalize(vec3(normalSample.x * 2.0 - 1.0, -(normalSample.y * 2.0 - 1.0), normalSample.z * 2.0 - 1.0)); for (uint tileLight = 0u; tileLight < count; ++tileLight) { int index = int(tileIndicesBuffer.tileIndices[tileIndex * 128 + tileLight]); GpuLight light = lightBuffer.lights[index]; float distanceToLight = distance(pixelPosition, light.positionRadius.xy); float falloff = 1.0 - smoothstep(0.0, max(light.positionRadius.z, 0.0001), distanceToLight); if (light.positionRadius.w > 0.5) { vec2 toPixel = normalize(pixelPosition - light.positionRadius.xy); float cone = dot(toPixel, light.shadowSoftness.yz); falloff *= smoothstep(light.shadowSoftness.w, light.positionRadius.w, cone); } float shadow = 1.0; if (index < lighting.shadowLightCount && falloff > 0.0) { vec4 header = shadowEdgesBuffer.shadowEdges[index]; int first = int(header.x); int count = int(header.y); if (count > 0) { float softness = max(light.shadowSoftness.x, 0.0); if (softness <= 0.0) { shadow = visibility(light.positionRadius.xy, pixelPosition, first, count); } else { shadow = 0.0; for (int offsetY = -1; offsetY <= 1; ++offsetY) for (int offsetX = -1; offsetX <= 1; ++offsetX) shadow += visibility(light.positionRadius.xy, pixelPosition + vec2(offsetX, offsetY) * softness, first, count); shadow /= 9.0; } } } float diffuse = 1.0; if (hasNormal) { vec3 toLight = normalize(vec3(light.positionRadius.xy - pixelPosition, lighting.lightHeight)); diffuse = max(dot(surfaceNormal, toLight), 0.0); } illumination += light.colourIntensity.rgb * falloff * light.colourIntensity.a * shadow * diffuse; } fragColor = vec4(base.rgb * illumination, base.a); }