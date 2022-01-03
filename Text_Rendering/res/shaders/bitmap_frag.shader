#version 330 core

in vec2 textCoord;

uniform sampler2D myTexture;
uniform  vec4 fontColor;

out vec4 fragColor;

void main() {
	vec4 textColor = texture(myTexture, textCoord) * fontColor;
	if (textColor.a < 0.1)
		discard;
	fragColor = textColor;
}