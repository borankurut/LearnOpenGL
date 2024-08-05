#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 verCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec2 aspectRatio1;
uniform vec2 aspectRatio2;

uniform float smileyAmount;

void main() {
	vec2 correctedTexCoord1 = aspectRatio1 * TexCoord;
	vec2 correctedTexCoord2 = aspectRatio2 * TexCoord;
	vec4 cami = texture2D(texture1, correctedTexCoord1);
	vec4 celal = texture2D(texture2, correctedTexCoord2);
	vec4 verColor = vec4(verCoord, 1.0f);
	vec4 newCelal = mix(celal, verColor, 0.5f);
	FragColor = mix(cami, newCelal, smileyAmount);

}


