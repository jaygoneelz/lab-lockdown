#version 330 core

layout (location = 0) in vec4 vertex; // (x, y, z, texCoord)
out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0); // Transform to clip space
    TexCoords = vertex.zw; // Pass texture coordinates to fragment shader
}