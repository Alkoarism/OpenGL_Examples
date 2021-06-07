#version 330 core

//in vec3 vertexColor;
in vec2 textCoord;

uniform sampler2D myTexture;
uniform sampler2D myTexture2;

out vec4 fragColor;

void main() {
	fragColor = mix(texture(myTexture, textCoord), texture(myTexture2, textCoord), 0.2f);
}
