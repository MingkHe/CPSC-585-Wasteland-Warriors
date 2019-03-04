#version 410

uniform sampler2D materialTex;

uniform int istex;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

void main() {
	if(istex == 1) {
		//vec4 surfaceColor = texture(materialTex, fragTexCoord);
		vec4 surfaceColor = vec4(1, 1, 1, 0);
		finalColor = surfaceColor;
	} else {
		finalColor = vec4(1, 0, 0, 0);
	}
}