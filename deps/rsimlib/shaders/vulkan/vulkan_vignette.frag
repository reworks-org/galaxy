#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform Vignette { layout(offset = 64) float radius; float softness; float intensity; } vignette;
layout(location = 0) in vec2 uv; layout(location = 0) out vec4 fragColor;
void main() { vec4 base = texture(source, uv); vec2 centred = uv - vec2(0.5); centred.x *= float(textureSize(source, 0).x) / float(textureSize(source, 0).y); float dist = length(centred); float inner = max(vignette.radius - vignette.softness, 0.0); float t = clamp((dist - inner) / max(vignette.softness, 0.0001), 0.0, 1.0); fragColor = vec4(base.rgb * (1.0 - t * vignette.intensity), base.a); }