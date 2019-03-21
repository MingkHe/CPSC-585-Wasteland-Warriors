#version 410

uniform sampler2D materialTex;

in vec2 fragTexCoord;
in vec3 fragVert;

out vec4 finalColor;

void main() {
	finalColor = texture(materialTex, fragTexCoord);
}
