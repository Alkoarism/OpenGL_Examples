#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTextCoord;
layout(location = 2) in vec4 aVertexColor;

out vec2 textCoord;
out vec4 vertexColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    textCoord = aTextCoord;
    vertexColor = aVertexColor;
}