#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	vec4 texColor1 = texture2D(texture1, TexCoord);
	vec4 texColor2 = texture2D(texture2, TexCoord);

	FragColor = mix(texColor1, texColor2, 0.2f);
}

