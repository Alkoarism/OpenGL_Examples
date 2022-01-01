#version 330 core

in vec2 textCoord;
in vec4 vertexColor;

uniform sampler2D myTexture;

out vec4 fragColor;

void main() {
	vec4 textColor = texture(myTexture, textCoord) * vertexColor;
	if (textColor.a < 0.1)
		discard;
	fragColor = textColor;
}