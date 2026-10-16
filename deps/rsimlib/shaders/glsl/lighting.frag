#version 430 core

uniform sampler2D source;
uniform int lightCount;
uniform int shadowLightCount;
uniform float ambient;
uniform int flipVertical;
uniform ivec2 tileCount;

struct GpuLight
{
    vec4 positionRadius;
    vec4 colourIntensity;
    vec4 shadowSoftness;
};

layout(std430, binding = 2) readonly buffer LightBuffer
{
    GpuLight lights[];
};

layout(std430, binding = 3) readonly buffer TileCounts
{
    uint tileCounts[];
};

layout(std430, binding = 4) readonly buffer TileIndices
{
    uint tileIndices[];
};

layout(std430, binding = 5) readonly buffer ShadowEdges
{
    vec4 shadowEdges[];
};

in vec2 uv;
out vec4 fragColor;

// 1.0 when the segment from the light to `target` is unobstructed by the light's shadow edges.
float visibility(vec2 lightPosition, vec2 target, int first, int count)
{
    vec2 ray = target - lightPosition;
    for (int edge = 0; edge < count; ++edge)
    {
        vec4 segment = shadowEdges[first + edge];
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

void main()
{
    vec2 lightUv = uv;
    if (flipVertical != 0)
    {
        lightUv.y = 1.0 - lightUv.y;
    }

    vec4 base = texture(source, uv);
    vec2 pixelPosition = min(lightUv * vec2(textureSize(source, 0)), vec2(textureSize(source, 0) - ivec2(1)));
    ivec2 tile = ivec2(pixelPosition / 16.0);
    int tileIndex = tile.y * tileCount.x + tile.x;
    uint tileLightCount = tileCounts[tileIndex];
    vec3 illumination = vec3(ambient);

    for (uint tileLight = 0u; tileLight < tileLightCount; ++tileLight)
    {
        int index = int(tileIndices[tileIndex * 128 + tileLight]);
        GpuLight light = lights[index];
        float distanceToLight = distance(pixelPosition, light.positionRadius.xy);
        float falloff = 1.0 - smoothstep(0.0, max(light.positionRadius.z, 0.0001), distanceToLight);

        if (light.positionRadius.w > 0.5)
        {
            vec2 toPixel = normalize(pixelPosition - light.positionRadius.xy);
            float cone = dot(toPixel, light.shadowSoftness.yz);
            falloff *= smoothstep(light.shadowSoftness.w, light.positionRadius.w, cone);
        }

        float shadow = 1.0;
        if (index < shadowLightCount && falloff > 0.0)
        {
            vec4 header = shadowEdges[index];
            int first = int(header.x);
            int count = int(header.y);
            if (count > 0)
            {
                float softness = max(light.shadowSoftness.x, 0.0);
                if (softness <= 0.0)
                {
                    shadow = visibility(light.positionRadius.xy, pixelPosition, first, count);
                }
                else
                {
                    shadow = 0.0;
                    for (int offsetY = -1; offsetY <= 1; ++offsetY)
                        for (int offsetX = -1; offsetX <= 1; ++offsetX)
                            shadow += visibility(light.positionRadius.xy, pixelPosition + vec2(offsetX, offsetY) * softness, first, count);
                    shadow /= 9.0;
                }
            }
        }

        illumination += light.colourIntensity.rgb * falloff * light.colourIntensity.a * shadow;
    }

    fragColor = vec4(base.rgb * illumination, base.a);
}