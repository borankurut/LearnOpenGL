#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec2 aspectRatio1;
uniform vec2 aspectRatio2;

uniform float smileyAmount;

void main() {
	vec2 correctedTexCoord1 = aspectRatio1 * TexCoord;
	vec2 correctedTexCoord2 = aspectRatio2 * TexCoord;
	FragColor = mix(texture2D(texture1, correctedTexCoord1), texture2D(texture2, correctedTexCoord2), smileyAmount);
}


