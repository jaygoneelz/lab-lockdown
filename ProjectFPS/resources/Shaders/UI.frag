#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text; // Glyph texture
uniform vec3 textColor; // Color of the text

void main() {    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r); // Sample the texture
    FragColor = vec4(textColor, 1.0) * sampled; // Apply text color
}
