#version 460 core
out vec4 FragColor;
flat in vec4 fColor;
void main()
{
    FragColor = fColor;
}