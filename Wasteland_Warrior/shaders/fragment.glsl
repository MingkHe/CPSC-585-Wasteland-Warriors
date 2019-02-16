// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;
//in vec2 texCoord;
//uniform sampler2DRect image;

uniform int shade;

in vec3 normal;
in vec3 lightVec;
in vec3 cameraVec;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void) {
    // write colour output without modification
    FragmentColour = vec4(Colour, 0);
	//FragmentColour = texture(image, texCoord);
	
	vec3 normLightVec = normalize(lightVec);
	vec3 reflection = 2*dot(normLightVec, normal) * normal - normLightVec;

	float shading = max(abs(dot(normalize(normal), normLightVec)), 0)//*5.f/pow(length(lightVec), 2.f)			//diffuse
	//+ pow(max(0.f, dot(reflection, normalize(cameraVec))), 10)*5.f//pow(length(lightVec), 2.f)	//specular
	+ .6f;																						//ambient
	if(shade == 0) {
		FragmentColour *= shading;
	} else if(shade == 1) {
		if(shading > .95f) {
			FragmentColour *= 1.f;
		} else if(shading > .6f) {
			FragmentColour *= .8f;
		} else if(shading > .4f) {
			FragmentColour *= .8f;
		} else {
			FragmentColour *= .5f;
		}
	}
	/*if(normal.x == 0.f && normal.y == 1.f && normal.z == 0.f) {
		FragmentColour = vec4(0, 1, 0, 0);
	}*/
}
