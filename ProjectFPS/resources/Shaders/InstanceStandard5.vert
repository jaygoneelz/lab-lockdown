#version 330 core

in vec3 aPos;
in vec3 aNormal;
in vec4 aColor0;
in vec3 aUV0;
in vec3 aTangent;
  
out VS_OUT {
    vec3 FragPos;
    vec4 Color;
    vec3 Normal;
    vec3 Tangent;
    vec3 TexCoord0;
    mat3 TBN;
} vsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 uv0Tilling;

uniform vec3 instancePositions[10];
uniform float instanceRotations[10];

void main()
{
    vec3 instanceOffset = instancePositions[gl_InstanceID];
    float rotationAngle = instanceRotations[gl_InstanceID];

    mat3 rotationMatrix = mat3(
    vec3(cos(rotationAngle), 0.0, -sin(rotationAngle)), 
    vec3(0.0, 1.0, 0.0),                               
    vec3(sin(rotationAngle), 0.0, cos(rotationAngle))  
    );

    vec3 rotatedPosition = rotationMatrix * aPos;

    vec4 worldPosition = model * vec4(rotatedPosition + instanceOffset, 1.0);
    gl_Position = projection * view * worldPosition;

    // Pass per-instance data to the fragment shader
    vsOut.FragPos = vec3(worldPosition);
    vsOut.TexCoord0 = aUV0 * uv0Tilling;
    vsOut.Color = aColor0;

    // Calculate normal and tangent with the model matrix
    vsOut.Normal = normalize(vec3(model * vec4(aNormal, 0.0)));
    vsOut.Tangent = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(cross(aNormal, aTangent), 0.0)));

    // Construct TBN matrix
    vsOut.TBN = mat3(vsOut.Tangent, B, vsOut.Normal);
}