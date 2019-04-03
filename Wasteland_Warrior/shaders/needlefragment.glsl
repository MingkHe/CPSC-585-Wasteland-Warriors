#version 410

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;
in vec3 fragColour;

out vec4 finalColor;

void main() {
        finalColor = vec4(fragColour, 1.f);
}
