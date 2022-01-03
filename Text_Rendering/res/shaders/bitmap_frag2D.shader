#version 330 core

in vec2 textCoord;

uniform sampler2D image;
uniform vec4 fragColor;

out vec4 fragColor;

void main() {
	fragColor = texture(image, textCoord) * fragColor;
}