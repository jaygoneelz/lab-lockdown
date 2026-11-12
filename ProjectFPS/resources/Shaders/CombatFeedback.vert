#version 330 core
out vec2 screenPoint;

void main()
{
    const vec2 vertices[3] = vec2[3](vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0));
    screenPoint = vertices[gl_VertexID];
    gl_Position = vec4(screenPoint, 0.0, 1.0);
}
