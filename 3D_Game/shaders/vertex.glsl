#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aOffset;

out vec4 fColor;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    fColor = aColor;
    gl_Position = uProjection * uView * (uTransform * vec4(aPosition + aOffset , 1.0));
    Normal = aNormal;
    FragPos = vec3(uTransform * vec4(aPosition + aOffset , 1.0));
}
