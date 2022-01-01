#version 330 core

in vec2 textCoord;

uniform sampler2D text;
uniform vec4 textColor;

out vec4 fragColor;

void main() {
	vec4 temp = texture(text, textCoord) * textColor;
	if (temp.w < 0.1)
		discard;
	fragColor = temp;
}
