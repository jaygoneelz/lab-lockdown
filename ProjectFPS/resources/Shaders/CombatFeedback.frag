#version 330 core
in vec2 screenPoint;
out vec4 color;

uniform vec3 startPoint;
uniform vec3 endPoint;
uniform vec3 effect;
uniform vec3 damageSide;
uniform vec3 impactColor;

void main()
{
    const vec2 aspect = vec2(16.0 / 9.0, 1.0);
    vec2 point = screenPoint * aspect;
    vec2 start = startPoint.xy * aspect;
    vec2 end = endPoint.xy * aspect;
    vec2 segment = end - start;
    float t = clamp(dot(point - start, segment) / max(dot(segment, segment), 0.000001), 0.0, 1.0);
    float tracer = effect.x * (1.0 - smoothstep(0.001, 0.007, length(point - start - t * segment)));
    float impact = effect.y * (1.0 - smoothstep(0.004, 0.042 - 0.024 * effect.y, length(point - end)));

    float edge = max(abs(screenPoint.x), abs(screenPoint.y));
    float roughness = 0.06 * sin(screenPoint.x * 37.0 + screenPoint.y * 18.0)
                    + 0.04 * sin(screenPoint.y * 63.0 - screenPoint.x * 22.0);
    float blood = effect.z * smoothstep(0.58, 0.98, edge + roughness) * clamp(0.65 + screenPoint.x * damageSide.x * 0.34, 0.25, 0.98);
    float shot = max(tracer, impact);
    float opacity = shot + blood * (1.0 - shot);
    vec3 rgb = impactColor * shot + vec3(0.65, 0.015, 0.02) * blood * (1.0 - shot);
    color = vec4(rgb / max(opacity, 0.0001), opacity);
}
